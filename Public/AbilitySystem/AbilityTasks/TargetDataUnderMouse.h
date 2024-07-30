// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

/* 
 * FGameplayAbilityTargetDataHandle은 TargetData를 관리하고 전달하는데 사용되는 구조체.
 * 다양한 유형의 TargetData를 래핑하여 Ability나 Effect가 필요로 하는 정보를 제공하는데 중요한 역할
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	// TargetDataUnderMouse 노드 생성 함수
	// HidePin: OwningAbility 핀을 노드에서 숨김
	// DefaultToSelf: OwningAbility 파라미터가 제공되지 않았을 때, 기본적으로 self 적용, 여기서는 OwningAbility로 적용
	// BlueprintInternalUseOnly: 참인 경우, 외부 블루프린트에서 이 함수를 호출할 수 있음
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BluepirntInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	// TargetDataUnderMouse 노드에 Output Execution Pin 생성
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:
	// 마우스 지점의 데이터를 얻고, Broadcast하기 위함
	// 델리게이트를 통해 Data를 Broadcast함
	virtual void Activate() override;
	void SendMouseCursorData();

	// 콜백함수, Activate와 TargetData 둘다 서버에서 만족할 경우 실행될 콜백 함수(DataHandle Broadcast하기 위한 함수)
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
