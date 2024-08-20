// Copyright Groot


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    // 상위 클래스인 AuraWidgetController의 내용이 비어있으므로 Super 필요없음

    const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
    OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
    OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
    OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
    // 부모 클래스에 코드없으므로 Super 필요없음
    const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnHealthChanged.Broadcast(Data.NewValue);
        }
    );
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnMaxHealthChanged.Broadcast(Data.NewValue);
        }
    );
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnManaChanged.Broadcast(Data.NewValue);
        }
    );
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnMaxManaChanged.Broadcast(Data.NewValue);
        }
    );

    // AuraAbilitySystemComponent에서 EffectAssetTag Broadcast 발생시 람다함수 실행
    Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
        [this](const FGameplayTagContainer& AssetTags)
        {
            // void UAuraAbilitySystemComponent::EffectApplied() 내의 for문 잘라서 붙여넣기
            for (const FGameplayTag& Tag : AssetTags)
            {
                // RequestGameplayTag(): 지정된 FName을 사용하여 FGameplayTag를 반환하는 스태틱 함수
                // "Mesage" 태그가 존재하는 경우 해당 태그 반환
                FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
                // 태그가 MessageTag와 일치하는지 확인, 일치할 경우 Row의 Broadcast 진행
                if (Tag.MatchesTag(MessageTag))
                {
                    // Boradcast에 반응하여 반복문을 통해 Tag에 따라 MessageWidgetDataTable의 row(행) 반환
                    const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
                    // FUIWidgetRow* Row 이므로  *Row 형태로 전달
                    MessageWidgetRowDelegate.Broadcast(*Row);
                }
            }
        }
    );
}