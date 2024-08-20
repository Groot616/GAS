// Copyright Groot


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
    UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
    // 반복문을 통해 순회하며 Attribute의 값이 변경될 경우 호출될 콜백을 바인딩함
    for (auto& Pair : AS->TagsToAttributes)
    {
        // GetGameplayAttributeValuechangeDelegate() : Attribute 값이 변경될 때 호출되는 델리게이트
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
            [this, Pair, AS](const FOnAttributeChangeData& Data)
            {
                BroadcastAttributeInfo(Pair.Key, Pair.Value());
            }
        );
    }
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
    // AttributeSet에 접근하기 위함
    UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

    check(AttributeInfo);

    // 반복문을 통해 AttributeSet의 TagsToAttributes 전체 확인
    for (auto& Pair : AS->TagsToAttributes)
    {
        BroadcastAttributeInfo(Pair.Key, Pair.Value());
    }
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
    // map의 key에 존재하는 태그를 통해 각 태그의 값을 얻어내고 Info에 저장
    FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
    // Info의 AttributeValue는 map의 밸류값에 존재하는 델리게이트를 실행시키고 얻은 값을 추출
        // GetNumericValue()의 파라미터로 어느 Attribute와 연관되어 있는지 설정
    Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
    // Info를 Broadcast
    AttributeInfoDelegate.Broadcast(Info);
}
