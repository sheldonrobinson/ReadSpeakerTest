// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PsychoActorComponent.generated.h"

/*
 * The basic emotions plus neutral
 */

UENUM(BlueprintType)
enum class Feeling : uint8
{
	NEUTRAL = 0,
	ANGER = 1,
	DISGUST = 2,
	SADNESS = 3,
	SURPRISE = 4,
	FEAR = 5,
	JOY = 6,
	TRUST = 7,
	ANTICIPATION = 8,
	MIXED = 9,
};

UENUM(BlueprintType)
enum class Emotion : uint8
{
	NEUTRAL = 0,
	ANGER = 1,
	DISGUST = 2,
	SADNESS = 3,
	SURPRISE = 4,
	FEAR = 5,
	JOY = 6,
	TRUST = 7,
	ANTICIPATION = 8,
	RAGE = 9,
	LOATHING = 10,
	GRIEF = 11,
	AMAZEMENT = 12,
	TERROR = 13,
	ECSTASY = 14,
	ADMIRATION = 15,
	VIGILANCE = 16,
	ANNOYANCE = 17,
	BOREDOM = 18,
	PENSIVENESS =19,
	DISTRACTION = 20,
	APPREHENSION = 21,
	SERENITY = 22,
	ACCEPTANCE = 23,
	INTEREST = 24,
	CONTEMPT = 25,
	REMORSE = 26,
	DISAPPROVAL = 27,
	AWE = 28,
	SUBMISSION = 29,
	OPTIMISM = 30,
	LOVE = 31,
	APPREHENSIVENESS = 32,
};
/**
 *  The basic facial expression to Neutral
 */
UENUM(BlueprintType)
enum class FacialPose : uint8
{
	NEUTRAL = 0,
	ANGER = 1,
	DISGUST = 2,
	SADNESS = 3,
	SURPRISE = 4,
    FEAR = 5,
	JOY = 6,
	CUSTOM = 7,
};

UENUM(BlueprintType)
enum class FacialExpression : uint8
{
	NEUTRAL = 0,
	ANGER = 1,
	DISGUST = 2,
	SADNESS = 3,
	SURPRISE = 4,
    FEAR = 5,
	JOY = 6,   
	RAGE = 7,
	REVULSION = 8,
	GRIEF = 9,
	SHOCK = 10,
	TERROR = 11,
	LAUGHTER = 12,
	INDIGNATION = 13,
	AVERSION = 14,
	MELANCHOLY =15,
	WONDER = 16,
	ANXIETY = 17,
	AMUSEMENT = 18,
	STERNNESS = 19,
	DISDAIN = 20,
	DEJECTION = 21,
	ALERTNESS = 22,
	CONCERN = 23,
	SATISFACTION = 24,
};

UENUM(BlueprintType)
enum class Behaviour : uint8
{
	NEUTRAL = 0,
	DISGUST = 1,
	CONTEMPT = 2,
	BELLIGERENCE = 3,
	DOMINEERING = 4,
	CRITICISM = 5,
	ANGER = 6,
	TENSION = 7,
	TENSE_HUMOR = 8,
	DEFENSIVENESS = 9,
	WHINING = 10,
	SADNESS = 11,
	STONEWALLING = 12,
	INTEREST = 13,
	VALIDATION = 14,
	AFFECTION = 15,
	HUMOR = 16,
	SURPRISE = 17,
	JOY = 18,
};

UENUM(BlueprintType)
enum class Gesture : uint8
{
	NEUTRAL = 0,
	ACKNOWLEDGE = 1,
	AGREEING = 2,
	ANGRY = 3,
	CONFUSED = 4,
	DISGUSTED = 5,
	DISMISSING = 6,
	EXCLAMATION = 7,
	ENTREAT = 8,
	GREETING = 9,
	HAPPY = 10,
	INTERESTED = 11,
	POINTING = 12,
	QUESTIONING = 13,
	SAD = 14,
	SURPRISED = 15,
	TENSE = 16,
	FEARFUL = 17,
};

UENUM(BlueprintType)
enum class BodyPosture : uint8
{
	NEUTRAL = 0,
	IDLE = 1,
	ANXIOUS = 2,
	DISTRESSED = 3,
	ANGRY = 4,
	DECEIVED = 5,
	SAD = 6,
	BORED = 7,
	REJECTED = 8,
	PESSIMISTIC = 9,
	ELATED = 10,
	ENJOYING = 11,
	EXCITED = 12,
	SURPRISED = 13,
	OVERCONFIDENT = 14,
	RELAXED = 15,
	RELIEF = 16,
	SATISFIED = 17,
	ALERTED = 18,
	CALM = 19,
	FATIGUED = 20,
};

USTRUCT(BlueprintType)
struct FMixedFeeling
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Psycho")
	Feeling Index = Feeling::NEUTRAL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Psycho")
	float Strength = 0.0f;
};

USTRUCT(BlueprintType)
struct FMixedLook
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Psycho")
	FacialPose Index = FacialPose::NEUTRAL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Psycho")
	float Strength = 0.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBehaviourEvent, Behaviour, behaviour, float, strength);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEmotionEvent, Emotion, emotion, float, strength);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFacialExpressionEvent, FacialExpression, expression);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFeelingEvent, Feeling, feeling);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGestureEvent, Gesture, gesture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPostureEvent, BodyPosture, posture);

// Pass array of Behaviour
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlendedBehaviourEvent, const TArray<float>&, behaviours); 
// Pass array of Feeling
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMixedFeelingsEvent, const TArray<FMixedFeeling>&, feelings);
// Pass array of FacialPose
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMixedLooksEvent, const TArray<FMixedLook>&, looks);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PSYCHO_API UPsychoActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

    UPROPERTY(BlueprintAssignable, Category = "Psycho|Behaviour")
	FOnBehaviourEvent OnBehaviourUpdate;
	
	UPROPERTY(BlueprintAssignable, Category = "Psycho|Behaviour")
	FOnBlendedBehaviourEvent OnBodyLanguageUpdate;
	
	UPROPERTY(BlueprintAssignable, Category = "Psycho|Behaviour")
	FOnPostureEvent OnPostureUpdate;
	
	UPROPERTY(BlueprintAssignable, Category = "Psycho|Behaviour")
	FOnGestureEvent OnGesture;
	
	UPROPERTY(BlueprintAssignable, Category = "Psycho|Emotion")
	FOnEmotionEvent OnEmotionChange;

	UPROPERTY(BlueprintAssignable, Category = "Psycho|Emotion")
	FOnFeelingEvent OnFeelingChange;
	
	UPROPERTY(BlueprintAssignable, Category = "Psycho|Emotion")
	FOnMixedFeelingsEvent OnFeelingsChange;
	
    UPROPERTY(BlueprintAssignable, Category = "Psycho|Emotion")
	FOnFacialExpressionEvent OnFacialExpressionUpdate;
	
	UPROPERTY(BlueprintAssignable, Category = "Psycho|Emotion")
	FOnMixedLooksEvent OnLookUpdate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Psycho|Emotion")
	Feeling Feeling = Feeling::NEUTRAL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Psycho|Emotion")
	Emotion Emotion = Emotion::NEUTRAL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Psycho|Emotion")
	FacialPose FacialPose = FacialPose::NEUTRAL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Psycho|Emotion")
	FacialExpression FacialExpression = FacialExpression::NEUTRAL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Psycho|Behaviour")
	BodyPosture BodyPosture = BodyPosture::NEUTRAL;
	
	// Sets default values for this component's properties
	UPsychoActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
