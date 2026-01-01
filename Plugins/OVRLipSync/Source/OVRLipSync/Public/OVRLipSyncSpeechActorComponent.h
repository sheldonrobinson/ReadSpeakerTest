// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/SpinLock.h"
#include "OVRLipSyncPlaybackActorComponent.h"
#include "OVRLipSyncContextWrapper.h"
#include "Sound/SoundWaveProcedural.h"
#include "OVRLipSyncSpeechActorComponent.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVisemesEvent, const TArray<float>&, visemes, float, laughter);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVRLIPSYNC_API UOVRLipSyncSpeechActorComponent : public UOVRLipSyncPlaybackActorComponent
{
	GENERATED_BODY()

public:

	UOVRLipSyncSpeechActorComponent(const FObjectInitializer &ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "LipSync|Speech")
	int32 SampleRate = 44100;

	UPROPERTY(EditAnywhere, Category = "LipSync|Speech", Meta = (ClampMin = "0", ClampMax = "100"))
	int32 Volume = 50;

	UPROPERTY(EditAnywhere, Category = "LipSync|Speech")
	int32 NumChannels = 1;

	UPROPERTY(EditAnywhere, Category = "LipSync|Speech")
	int32 BufferSize = 4096;

	UPROPERTY(EditAnywhere, Category = "LipSync|Speech")
	OVRLipSyncProviderKind ProviderKind = OVRLipSyncProviderKind::EnhancedWithLaughter;

	UPROPERTY(EditAnywhere, Category = "LipSync|Speech", Meta = (ToolTip = "Enable hardware acceleration on supported platforms"))
	bool EnableHardwareAcceleration = true;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "LipSync|Speech", Meta = (ToolTip = "Feed AudioBuffer containing packaged mono 16-bit signed integer PCM values"))
	void FeedAudio(const TArray<uint8>& AudioData);

	UFUNCTION(BlueprintCallable, Category = "LipSync|Speech", Meta = (ToolTip = "Initialize Audio"))
	void InitializeAudio(UAudioComponent * InAudioComponent);



protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UFUNCTION(BlueprintCallable, Category = "LipSync|Audio")
	USoundWave *ByteArrayToSoundWave(const TArray<uint8> &DataArray);

private:
	int32_t ChunkSampleSize;
	int32_t ChunkSize;
	int32_t FrameOffset = 0;
	int32_t FrameDelayInMs = 0;
	bool bStereo = false;
	bool bIsSpeaking = false;



	TSharedPtr<UOVRLipSyncContextWrapper> LipSyncContext;
	TQueue<TArray<uint8>> VoiceAudioData;
 	TQueue<TPair<UOVRLipSyncFrameSequence *, TArray<int16>>> lipSyncData;
	void CookAudioData(const TArray<uint8>& PCMData);
	void TryResumePlayback();

	UFUNCTION()
	void OnAudioPlaybackFinished();
	UFUNCTION()
	void OnAudioPercentPlayedUpdated(const USoundWave *PlayingSoundWave, const float PlaybackPercent);

	UE::FSpinLock VoiceLock;
	UE::FSpinLock PlaybackLock;
		
};
