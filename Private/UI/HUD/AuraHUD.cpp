// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		// NewObject 는 새로운 UObject를 생성
		// this는 outer를 AuraHUD 자신이 되도록
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 유효값 검사
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	// 스크린에 표시하기 위한 WBP_OverlayWidget 생성후, AddToViewport() 함수를 통해 뷰포트에 출력
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	// OverlayWidget에 OverlayWidgetController를 셋하기 위해 캐스팅
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	// 구조체 생성 및 초기화
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	// GetOverWidgetController() 함수를 통해 OverlayWidgetController를 리턴받음
	// 파라미터는 OverlayWidgetController가 유효하지 않을 경우 파라미터값으로 초기화해서 리턴
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// OverlayWidget의 컨트롤러를 WidgetController로 셋
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}


