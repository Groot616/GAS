// Copyright Groot

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FAuraGameplayTags
{
public:
	// 클래스의 싱글톤 인스턴스를 반환하는 함수
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	// GameplayTag 초기화 함수
	static void InitializeNativeGameplayTags();


    /*
     * Primary Attributes Tags
     */
    FGameplayTag Attributes_Primary_Strength;
    FGameplayTag Attributes_Primary_Intelligence;
    FGameplayTag Attributes_Primary_Resilience;
    FGameplayTag Attributes_Primary_Vigor;

    /*
     * Secondary Attributes Tags
     */
     // Armor만 제외한 나머지는 전부 추가된 코드
    FGameplayTag Attributes_Secondary_Armor;
    FGameplayTag Attributes_Secondary_ArmorPenetration;
    FGameplayTag Attributes_Secondary_BlockChance;
    FGameplayTag Attributes_Secondary_CriticalHitChance;
    FGameplayTag Attributes_Secondary_CriticalHitDamage;
    FGameplayTag Attributes_Secondary_CriticalHitResistance;
    FGameplayTag Attributes_Secondary_HealthRegeneration;
    FGameplayTag Attributes_Secondary_ManaRegeneration;
    FGameplayTag Attributes_Secondary_MaxHealth;
    FGameplayTag Attributes_Secondary_MaxMana;

    /*
     * Resistances
     */
    FGameplayTag Attributes_Resistance_Fire;
    FGameplayTag Attributes_Resistance_Lightning;
    FGameplayTag Attributes_Resistance_Arcane;
    FGameplayTag Attributes_Resistance_Physical;

    /*
     *InputTags
     */
    FGameplayTag InputTag_LMB;
    FGameplayTag InputTag_RMB;
    FGameplayTag InputTag_1;
    FGameplayTag InputTag_2;
    FGameplayTag InputTag_3;
    FGameplayTag InputTag_4;

    FGameplayTag Damage;
    /*
     * Damage Types
     */
    FGameplayTag Damage_Fire;
    FGameplayTag Damage_Lightning;
    FGameplayTag Damage_Arcane;
    FGameplayTag Damage_Physical;

    TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

    /*
     * Effects
     */
    FGameplayTag Effects_HitReact;

protected:

private:
	static FAuraGameplayTags GameplayTags;
};