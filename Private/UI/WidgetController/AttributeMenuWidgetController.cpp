// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	// 반복문을 통해 순회하며 Attribute의 값이 변경될 경우 호출될 콜백을 바인딩함
	for (auto& Pair : AS->TagsToAttributes)
	{
		// GetGameplayAttributeValuechangeDelegate() : Attribute 값이 변경될 때 호출되는 델리게이트
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
				//// FAuraAttributeInfo 타입 구조체 Info 생성후 Tag저장
				//FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
				//// Info에 값을 수치화하여 저장, 어느 Attribute와 연관되어 있는지 설정
				//Info.AttributeValue = Pair.Value().GetNumericValue(AS);
				//// Info를 Broacast
				//AttributeInfoDelegate.Broadcast(Info);
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	// AS 캐스팅
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	// 반복문을 통해 AttributeSet의 TagssToAttributes 전체 확인
	for (auto& Pair : AS->TagsToAttributes)
	{	
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
		//FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		//// Info에 값을 수치화하여 저장, 어느 Attribute와 연관되어 있는지 설정
		//Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		//// Info를 Broacast
		//AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	// FAuraAttributeInfo 타입 구조체 Info 생성후 Tag저장
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	// Info에 값을 수치화하여 저장, 어느 Attribute와 연관되어 있는지 설정
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	// Info를 Broacast
	AttributeInfoDelegate.Broadcast(Info);
}
