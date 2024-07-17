// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;


/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	// 기존 에지터에서 사용한 mapping과 동일한 역할
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	// 레퍼런스를 매개변수로 받기 때문에, 값변경을 방지하기 위한 const 선언
	// FInputActionValue 사용을 위한 전방선언 필요
	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();
	/**
	* 기존 포인터 방식이 아닌 TObjectPtr와 유사한 새로운 방식, 캐스팅 필요x
	*/
	// 틱 변경전 마우스커서가 가리키는 액터
	TScriptInterface<IEnemyInterface> LastActor;
	// 틱 변경후 마우스커서가 가리키는 액터
	TScriptInterface<IEnemyInterface> ThisActor;

};
