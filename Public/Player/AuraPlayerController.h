// Copyright Groot

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;

struct FInputActionValue;
struct FGameplayTag;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	// 서버가 캐릭터 컨트롤시 서버에서 함수를 실행시키고 서버측에서 데미지 보임
	// 클라이언트가 캐릭터 컨트롤시 서버에서 함수를 실행시키지만 클라이언트측에서 데미지 보임
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	// 기존 에디터에서 사용한 매핑과 동일한 역할
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	// 레퍼런스를 매개변수로 받기 때문에, 값변경을 방지하기 위한 const 선언
	// FInputActionValue 사용을 위한 전방선언 필요
	void Move(const FInputActionValue& InputActionValue);

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	bool bShiftKeyDown = false;
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };

	void CursorTrace();
	/**
	* 기존 포인터 방식이 아닌 TObjectPtr와 유사한 새로운 방식, 캐스팅 필요x
	*/
	// 틱 변경전 마우스커서가 가리키는 액터
	TScriptInterface<IEnemyInterface> LastActor;
	// 틱 변경후 마우스커서가 가리키는 액터
	TScriptInterface<IEnemyInterface> ThisActor;

	FHitResult CursorHit;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	// FAuraInputAction 구조체 사용을 위한 변수
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	// RMB 클릭시 클릭지점을 저장할 벡터
	FVector CachedDestination = FVector::ZeroVector;
	// 클릭 유지시간
	float FollowTime = 0.f;
	// 클릭 최소 요구지속 시간, 0.5초 이내의 클릭 지속은 무시하기 위함
	float ShortPressThreshold = 0.5f;
	// 클릭 최소 요구시간을 만족하지 않으면 자동이동이 발생하지 않도록 하기 위함
	bool bAutoRunning = false;
	// 적을 타게팅하고 있는지 확인하기 위함
	bool bTargeting = false;

	// 오차범위 조정용, 해당 반경 내로 들어오면 도달한 것으로 판단
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	// 캐릭터 이동시 자연스러운 곡선 이동을 하기 위한 Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	// 클릭 지점 자동 이동
	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
