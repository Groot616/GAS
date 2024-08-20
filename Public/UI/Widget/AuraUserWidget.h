// Copyright Groot

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
	// WidgetController를 설정하고, WidgetControllerSet()[=BeginPlay()] 를 호출함를 호출하게 됨
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	// 블루프린트에서 접근할 수 있어야 함
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	// WidgetController가 설정되자마자 호출, BeginPlay()와 동일한 역할을 하는 함수
	// BlueprintImplementableEvent를 통해 위젯에서 위젯 컨트롤러를 설정할 때마다 호출됨
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
