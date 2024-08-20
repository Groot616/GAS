// Copyright Groot

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

// 적용 시점 지정
UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	// 오버랩시 GameplayEffect 적용
	ApplyOnOverlap,

	// 오버랩 종료시 GameplayEffect 적용
	ApplyOnEndOverlap,

	// GameplayEffet 미적용
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

	// 파라미터로 타겟과 GameplayEffect를 받음으로써 해당 타겟에게 변화를 적용시킬 예정
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	// 오버랩시 GameplayEffect Policy에 따라 GameplayEffectd 적용
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	// 오버랩 종료시 GameplayEffect Policy에 따라 GameplayEffect 종료
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

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
	// InfiniteEffect Remove 적용 policy, 오버랩 끝날때로 적용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	// 현재 적용중인 GameplayEffect와 적용대상의 ASC map
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	float ActorLevel = 1.f;

private:

};
