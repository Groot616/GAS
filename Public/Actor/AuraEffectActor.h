  // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

// GameplayEffect 적용 Policy
UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	// 오버랩시 GameplayEffect 적용
	ApplyOnOverlap,

	// 오버랩 종료시 GameplayEffect 적용
	ApplyOnEndOverlap,

	// GameplayEffect 미적용
	DoNotApply
};

// GameplayEffect 종료 Policy
UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	// 오버랩 종료시 GameplayEffect 종료
	RemoveOnEndOverlap,

	// 종료 미적용
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraEffectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Effect를 Target에게 적용시키는 함수
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect>GameplayEffectClass);

	// 오버랩시 GameplayEffect Policy에 따라 GameplayEffect 적용
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	// 오버랩 종료시 GameplayEffect Policy에 따라 GameplayEffect 종료
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectRemoval = false;

	// 적용시길 GameplayEffect 클래스, GameplayEffect 기반 블루프린트 생성후 적용예정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	// InstantEffect 적용 policy, 기본값 : 미적용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	// 현재 적용중인 GameplayEffect와 적용대상의 ASC map
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	float ActorLevel = 1.f;

private:
	


};
