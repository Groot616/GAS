// Copyright Groot

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/** Combat Interface */

protected:

private:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	UCameraComponent* Camera;

	virtual void InitAbilityActorInfo() override;
};
