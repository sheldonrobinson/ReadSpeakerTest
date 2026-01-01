// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/SpinLock.h"
#include "ReadSpeakerTTS.h"
#include "Components/ActorComponent.h"
#include "TTSVoiceComponent.generated.h"

USTRUCT(BlueprintType)
struct READSPEAKERTTS_API FAudioFragment {

	GENERATED_BODY()

	FAudioFragment() { }

	FAudioFragment(TArray<uint8> InAudio) : Audio(InAudio) { }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReadSpeaker|Voice" )
	TArray<uint8> Audio;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAudioEvent, const FAudioFragment&, audio);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DefaultToInstanced)
class READSPEAKERTTS_API UTTSVoiceComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "ReadSpeaker|Voice")
	FOnAudioEvent OnAudio;

	// Sets default values for this component's properties
	UTTSVoiceComponent(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
     * Sets the engine to be used by this speaker.
     */
	UFUNCTION(BlueprintCallable, Category = "ReadSpeaker|Voice", meta = (Keywords = "SetSpeaker", DefaultToSelf))
	void SetSpeaker(UTTSSpeaker* Speaker);

	/**
	 * Gets the Engine currently used by this speaker.
	 */
	UFUNCTION(BlueprintCallable, Category = "ReadSpeaker|Voice", meta = (Keywords = "GetSpeaker", DefaultToSelf))
	UTTSSpeaker* GetSpeaker();

	/**
	 * Synthesize audio using the settings of this speaker.
	 * @param {FString} text The text to be read.
	 * @param {TTSTextType} textType The format of text, either SSML or Normal.
	 */
	UFUNCTION(BlueprintCallable, Category = "ReadSpeaker|Voice", meta = (Keywords = "Voice", DefaultToSelf))
	void voice(FString text = "", TTSTextType textType = TTSTextType::Normal);

	/**
	 * Synthesize audio using the settings of this speaker. Conversion happens asynchronously
	 * @param {FString} text The text to be read.
	 * @param {TTSTextType} textType The format of text, either SSML or Normal.
	 */
	UFUNCTION(BlueprintCallable, Category = "ReadSpeaker|Voice", meta = (Keywords = "VoiceAysnc", DefaultToSelf))
	void voiceAsync(FString text = "", TTSTextType textType = TTSTextType::Normal);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

private:
	UPROPERTY(Transient)
	UTTSSpeaker* Speaker;

	UPROPERTY(Transient)
	UTTSConverter* Converter;

	UFUNCTION()
	void GrabAudio();

	TQueue<TArray<int16>> Audio;
	UE::FSpinLock AudioLock;

	FRWLock AudioDataLock;
};
