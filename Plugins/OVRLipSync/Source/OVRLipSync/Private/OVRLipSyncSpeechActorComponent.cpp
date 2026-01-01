// Fill out your copyright notice in the Description page of Project Settings.


#include "OVRLipSyncSpeechActorComponent.h"
#include "OVRLipSyncContextWrapper.h"
#include "OVRLipSyncModule.h"


USoundWave *UOVRLipSyncSpeechActorComponent::ByteArrayToSoundWave(const TArray<uint8> &DataArray)
{

	USoundWave *SoundWave = NewObject<USoundWave>(USoundWave::StaticClass());
	if (!ensure(SoundWave))
	{
		UE_LOG(LogOvrLipSync, Error, TEXT("UOVRLipSyncSpeechActorComponent:ByteArrayToSoundWave: SoundWave could not be created."));
		return nullptr;
	}

	const uint32 Num = DataArray.Num();
	SoundWave->Duration = ((double)Num * (double)NumChannels) / (double)SampleRate;

	SoundWave->SetSampleRate(SampleRate / 2);
	SoundWave->NumChannels = NumChannels;
	SoundWave->SoundGroup = ESoundGroup::SOUNDGROUP_Voice;
	SoundWave->Volume = (double)Volume / 100.0;
	SoundWave->RawPCMDataSize = Num * sizeof(uint8);
	SoundWave->RawPCMData = (uint8 *)FMemory::Malloc(Num * sizeof(uint8));
	FMemory::Memcpy(SoundWave->RawPCMData, DataArray.GetData(), Num * sizeof(uint8));

	return SoundWave;
}

void UOVRLipSyncSpeechActorComponent::CookAudioData(const TArray<uint8> &PCMData)
{
	TArray<int16> PCMData16(reinterpret_cast<const int16 *>(PCMData.GetData()), PCMData.Num() / 2);
	int32 PCMDataSize = PCMData16.Num();
	const int16 *PCMDataInt16 = PCMData16.GetData();

	UOVRLipSyncFrameSequence *NewSequence = NewObject<UOVRLipSyncFrameSequence>();

	if (!LipSyncContext)
	{
		UE_LOG(LogOvrLipSync, Error, TEXT("LipSyncContext is NULL"));
		return;
	}

	float NewLaughterScore = 0.0f;

	TArray<float> NewVisemes;

	TArray<int16> samples;
	samples.SetNumZeroed(ChunkSize);

	for (int32 Offset = 0; Offset < PCMDataSize + FrameOffset; Offset += ChunkSize)
	{
		const int32 RemainingSamples = PCMDataSize - Offset;
		if (RemainingSamples >= ChunkSize)
		{
			LipSyncContext->ProcessFrame(PCMDataInt16 + Offset, ChunkSampleSize, NewVisemes, NewLaughterScore,
										 FrameDelayInMs, bStereo);
		}
		else
		{
			if (RemainingSamples > 0)
			{
				memcpy(samples.GetData(), PCMDataInt16 + Offset, sizeof(int16) * RemainingSamples);
				memset(samples.GetData() + RemainingSamples, 0, ChunkSize - RemainingSamples);
			}
			else
			{
				memset(samples.GetData(), 0, ChunkSize);
			}
			LipSyncContext->ProcessFrame(samples.GetData(), ChunkSampleSize, NewVisemes, NewLaughterScore,
										 FrameDelayInMs, bStereo);
		}

		if (Offset >= FrameOffset)
		{
			NewSequence->Add(NewVisemes, NewLaughterScore);
		}
	}
	lipSyncData.Enqueue(TPair<UOVRLipSyncFrameSequence *, TArray<int16>>(NewSequence, PCMData16));
}

UOVRLipSyncSpeechActorComponent::UOVRLipSyncSpeechActorComponent(const FObjectInitializer &ObjectInitializer)
	: UOVRLipSyncPlaybackActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	

}

// Called when the game starts
void UOVRLipSyncSpeechActorComponent::BeginPlay()
{
	Super::BeginPlay();

	bStereo = NumChannels > 1;
	ChunkSampleSize = (SampleRate/2) / 100; // Scaled down by 100Hz
	ChunkSize = NumChannels * ChunkSampleSize;

	LipSyncContext = MakeShared<UOVRLipSyncContextWrapper>(UOVRLipSyncContextWrapper::ContextProviderFromProviderKind(ProviderKind), SampleRate,
		BufferSize, FString(), EnableHardwareAcceleration);

	float InLaughterScore = 0.0f;
	TArray<float> InVisemes;
	TArray<int16_t> samples;
	samples.SetNumZeroed(ChunkSize);
	LipSyncContext->ProcessFrame(samples.GetData(), ChunkSampleSize, InVisemes, InLaughterScore, FrameDelayInMs, bStereo);
	FrameOffset = (int32_t)(FrameDelayInMs * (SampleRate / 2) / 1000 * NumChannels);

	bIsSpeaking = false;
}

void UOVRLipSyncSpeechActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	LipSyncContext = nullptr;
	lipSyncData.Empty();
	VoiceAudioData.Empty();
	Super::EndPlay(EndPlayReason);
}

void UOVRLipSyncSpeechActorComponent::InitializeAudio(UAudioComponent *InAudioComponent)
{
	if (!InAudioComponent)
	{
		UE_LOG(LogOvrLipSync, Error, TEXT("UOVRLipSyncSpeechActorComponent:InitializeAudio: AudioComponent cannot be NULL"));
	}
	TArray<uint8> EmptyByteAudio;
	EmptyByteAudio.Init(0, 1024);
	InAudioComponent->SetSound(ByteArrayToSoundWave(EmptyByteAudio));
	InAudioComponent->OnAudioFinished.AddDynamic(this, &UOVRLipSyncSpeechActorComponent::OnAudioPlaybackFinished);
	InAudioComponent->OnAudioPlaybackPercent.AddDynamic(this, &UOVRLipSyncSpeechActorComponent::OnAudioPercentPlayedUpdated);
	Start(InAudioComponent, NULL);
}

void UOVRLipSyncSpeechActorComponent::OnAudioPercentPlayedUpdated(const USoundWave* PlayingSoundWave, const float PlaybackPercent) {
	if (FMath::IsNearlyEqual(1.0f, PlaybackPercent))
	{
		if (lipSyncData.IsEmpty())
		{
			bIsSpeaking = false;
			OnAudioPlaybackFinished();
		}
		else
		{
			TryResumePlayback();
		}
	}
}

void UOVRLipSyncSpeechActorComponent::OnAudioPlaybackFinished()
{
	if (lipSyncData.IsEmpty())
	{
		Sequence = nullptr;
		InitNeutralPose();
	}
}

// Called every frame
void UOVRLipSyncSpeechActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
									   FActorComponentTickFunction *ThisTickFunction)
{
	if (!VoiceAudioData.IsEmpty())
	{
		if (VoiceLock.TryLock())
		{
			while (!VoiceAudioData.IsEmpty())
			{
				TArray<uint8> sound;
				if (VoiceAudioData.Dequeue(sound))
				{
					CookAudioData(sound);
				}
			}
			VoiceLock.Unlock();
		} 
	}

	if (!lipSyncData.IsEmpty() && !bIsSpeaking)
	{
		TryResumePlayback();
	}
}

void UOVRLipSyncSpeechActorComponent::FeedAudio(const TArray<uint8>& VoiceData)
{
	if (!LipSyncContext)
	{
		UE_LOG(LogOvrLipSync, Error, TEXT("UOVRLipSyncSpeechActorComponent:FeedAudio: LipSyncContext is NULL"));
		return;
	}
	VoiceAudioData.Enqueue(VoiceData);
}

void UOVRLipSyncSpeechActorComponent::TryResumePlayback() {
	if (!lipSyncData.IsEmpty())
	{
		if (PlaybackLock.TryLock())
		{
			TPair<UOVRLipSyncFrameSequence *, TArray<int16>> newLipSyncSequence;
			if (lipSyncData.Dequeue(newLipSyncSequence))
			{
				TArray<uint8> AudioData(reinterpret_cast<const uint8 *>(newLipSyncSequence.Value.GetData()),
										newLipSyncSequence.Value.Num() * 2);
				SetPlaybackSequence(newLipSyncSequence.Key);				
				AudioComponent->SetSound(ByteArrayToSoundWave(AudioData));
				AudioComponent->Play();
				bIsSpeaking = true;
			}
			PlaybackLock.Unlock();
		}
	}
}