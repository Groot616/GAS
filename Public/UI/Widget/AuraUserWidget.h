// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// WidgetController를 설정하고, WidgetControllerSet()[=BeginPlay()] 를 호출함
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	// View의 value를 변경하기 위한 BlueprintReadOnly
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	// WidgetController가 설정되자마자 호출, BeginPlay()와 동일한 역할을 하는 함수
	// BlueprintImplementableEvent를 통해 위젯에서 위젯 컨트롤러를 설정할 때마다 호출됨
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
