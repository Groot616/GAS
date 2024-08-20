// Copyright Groot

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
    // AuraProjectileSpell.cpp에 있는 코드와 동일
    /*
     * MakeEffectContext() 함수에서 FGameplayEffectContextHandle 타입의 구조체 생성시 파라미터로 해당 함수 사용
     * FGameplayEffectContextHandle Context = FGameplayEffectContextHandle(UAbilitySystemGlobals::Get().AllocGameplayEffectContext());
     */
    virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
