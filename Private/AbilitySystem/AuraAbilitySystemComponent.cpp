// Copyright Groot


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "GameplayTagContainer.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);

    // FAuraGameplayTags에서 GameplayTag를 가져오기 위한 코드
    const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
    //GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Secondary_Armor.ToString()));
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
    // 반복문을 통해 Ability들에 대한 Spec을 만들고 Ability 사용을 가능하도록 함
    for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
        {
            // GameplayAbilitySpec에는 게임 진행 도중에 동적으로 추가되거나 제거될 수 있는 태그 전용 GameplayTagContainer가 있음
            // 그리고 AbilitySystemComponent가 게임 시작시 처음으로 Ability를 추가할 때
            // 이러한 능력들은 StartupAbilities이므로 StartupInputTag를 통해 확인할 수 있고
            // 해당 Ability에 대해 이 태그들을 AbilitySpec에 추가 가능함
            /*
             * AbilitySpec: 특정 Ability에 대한 사양을 담고 있는 객체
             * DynamicAbilityTags: 동적으로 추가되는 태그
             * AddTag: 새로운 태그 추가
             * AuraAbiltiy->StartupInputTag: 추가할 태그(LMB, RMB, 1, 2, 3, 4)
             */
            AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
            GiveAbility(AbilitySpec);
        }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return;

    // GetActivatableAbilities(): 활성화 가능한 모든 Ability들을 체크하는 함수, TArray 리턴
    // 루프를 통한 활성화 가능한 Ability를 순회
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {

        // 일치하는 태그가 있을 경우, Ability가 활성화되지 않은 경우에만 Ability를 활성화하도록 함     
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            // 입력이 시작되었다는 것을 알림
            AbilitySpecInputPressed(AbilitySpec);
            if (!AbilitySpec.IsActive())
            {
                TryActivateAbility(AbilitySpec.Handle);
            }
        }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return;

    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            // 키 입력이 종료되었다는 것을 알림
            AbilitySpecInputReleased(AbilitySpec);
        }
    }
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
    //GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied"));
    // 스크린 디버그 메세지 코드 삭제
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);
    EffectAssetTags.Broadcast(TagContainer);
}
