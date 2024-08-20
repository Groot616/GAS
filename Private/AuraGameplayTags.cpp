// Copyright Groot


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

// static 변수는 정확하게 한 번만 생성되고 초기화되도록 명시적으로 타입 선언을 해주어야 함
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// Get() 함수를 통해 Manager 호출, AddNativeGameplayTag()를 통해 태그와 설명 추가
    /*
     * Primary Attributes
     */
    GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Strength"), FString("Increases physical damage"));

    GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Intelligence"), FString("Increases magical damage"));

    GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Resilience"), FString("Increases Armor and Armor Penetration"));

    GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Vigor"), FString("Increases Health"));

    /*
     * Secondary Attriubtes
     */
     // Armor만 제외한 나머지는 전부 추가된 코드
    GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.Armor"), FString("Reduce damage taken, improves Block Chance"));

    GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.ArmorPenetration"), FString("Ignore Percentage of enemy Armor, Increases Critical Hit Chance"));

    GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.BlockChance"), FString("Chance to cut incoming damage in half"));

    GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CriticalHitChance"), FString("Chance to double damage plus critical hit bonus"));

    GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CriticalHitDamage"), FString("Bonus damage added when a critical hit is scored"));

    GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces Critical Hit Chance of attacking enemies"));

    GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.HealthRegeneration"), FString("Amount of Health regenerated every 1 second"));

    GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.ManaRegeneration"), FString("Amount of Mana regenerated every 1 second"));

    GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MaxHealth"), FString("Maximum amount of Health obtainable"));

    GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MaxMana"), FString("Maximum amount of Mana obtainable"));

    /*
     * InputTags
     */
    GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.LMB"), FString("Input Tag for Left Mouse Button"));

    GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.RMB"), FString("Input Tag for Right Mouse Button"));

    GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.1"), FString("Input Tag for 1 key"));

    GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.2"), FString("Input Tag for 2 key"));

    GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.3"), FString("Input Tag for 3 key"));

    GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.4"), FString("Input Tag for 4 key"));

    GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Damage"), FString("Damage"));

    /*
     * Damage Types
     */
    GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Damage.Fire"), FString("Fire Damage Type"));

    GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Damage.Lighting"), FString("Lightning Damage Types"));

    GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Damage.Arcane"), FString("Arcane Damage Types"));

    GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Damage.Physical"), FString("Physical Damage Types"));

    /*
     * Resistances
     */
    GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Resistance.Fire"), FString("Resistance to Fire damage"));

    GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Resistance.Lightning"), FString("Resistance to Lightning damage"));

    GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Resistance.Arcane"), FString("Resistance to Arcane damage"));

    GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Resistance.Physical"), FString("Resistance to Physical damage"));

    /*
     * Map of Damage Types to Resistance
     */
    GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
    GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
    GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
    GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);

    /*
     * Effects
     */
    GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Effects.HitReact"), FString("Tag granted when Hit Reacting"));
}