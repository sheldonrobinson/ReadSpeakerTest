// Fill out your copyright notice in the Description page of Project Settings.


#include "TTSVoiceComponent.h"

// Sets default values for this component's properties
UTTSVoiceComponent::UTTSVoiceComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}


// Called when the game starts
void UTTSVoiceComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTTSVoiceComponent::BeginDestroy() {
	if (Converter != nullptr) {
		UTTSEngine* engine = Converter->Engine;
		if(engine != nullptr) {
			engine->UnloadEngine();
		}
	}
	Super::BeginDestroy();
}


// Called every frame
void UTTSVoiceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!Audio.IsEmpty()) {
		TArray<int16> pcm;
		if (AudioLock.TryLock()) {
			while (!Audio.IsEmpty()) {
				TArray<int16> sound;
				if (Audio.Dequeue(sound)) {
					pcm.Append(sound);
				}
			}
			AudioLock.Unlock();
		}
		pcm.Shrink();
		TArray<uint8> data(reinterpret_cast<const uint8*>(pcm.GetData()), pcm.Num() * 2);
		FAudioFragment audio(data);
		OnAudio.Broadcast(audio);
		UE_LOG(LogReadSpeakerTTS, Display, TEXT("UTTSVoiceComponent::TickComponent sent %d bytes of audio."), audio.Audio.Num());
	}
}

void UTTSVoiceComponent::SetSpeaker(UTTSSpeaker* NewSpeaker) {
	if (NewSpeaker != nullptr) {
		Speaker = NewSpeaker;
		Converter = NewObject<UTTSConverter>();
		{
			UTTSEngine* engine = Speaker->GetEngine();
			if (engine != nullptr) {
				engine->LoadEngine();
				Converter->Engine = engine;
			}
			else {
				UE_LOG(LogReadSpeakerTTS, Error, TEXT("Engine was null")); 
			}
	    }
		Converter->Volume = Speaker->GetVolume();
		Converter->Pitch = Speaker->GetPitch();
		Converter->Speed = Speaker->GetSpeed();
		Converter->Pause = Speaker->GetPause();
		Converter->CommaPause = Speaker->GetCommaPause();
		Converter->AudioComponent = Speaker->GetAudioComponent();
		Converter->OutputFormat = TTSOutputFormat::PCM16;
		Converter->SoundAttenuationSettings = &(Speaker->SoundAttenuation);
		Converter->OnConversionFinished.AddDynamic(this, &UTTSVoiceComponent::GrabAudio);
		
	}
	else {
		UE_LOG(LogReadSpeakerTTS, Error, TEXT("Speaker was null"));
	}
}

UTTSSpeaker* UTTSVoiceComponent::GetSpeaker()
{
	if (Speaker == nullptr) {
		UE_LOG(LogReadSpeakerTTS, Error, TEXT("Speaker was null"));
	}
	return Speaker;
}

void UTTSVoiceComponent::voice(FString text, TTSTextType textType) {
	AudioDataLock.WriteLock();
	Converter->Text = text;
	Converter->TextType = textType;
	Converter->ConvertToBuffer();
	AudioDataLock.WriteUnlock();
}

void UTTSVoiceComponent::voiceAsync(FString text, TTSTextType textType) {
	AudioDataLock.WriteLock();
	Converter->Text = text;
	Converter->TextType = textType;
	Converter->ConvertToBufferAsync();
	AudioDataLock.WriteUnlock();
}

void UTTSVoiceComponent::GrabAudio() {
	if (Converter != nullptr) {
		AudioDataLock.ReadLock();
		TArray<int16> sound = Converter->GetAudioData();
		if (!sound.IsEmpty()) {
			Audio.Enqueue(sound);
			Converter->ClearAudioData();
		}
		AudioDataLock.ReadUnlock();
	}
}
