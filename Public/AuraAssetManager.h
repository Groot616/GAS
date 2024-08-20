// Copyright Groot

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	// 클래스의 싱글톤 인스턴스를 반환하는 함수
	static UAuraAssetManager& Get();

protected:
	// AuraGameplayTags 클래스의 InitializeNativeGameplayTags() 함수와 동일한 역할
	virtual void StartInitialLoading();
};
