// Copyright Groot

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 선언된 함수를 0으로 초기화함으로써 pure function으로 만들고
	// EnemyInterface 클래스에서 정의하는 것이 아닌 Enemy 클래스에서 정의
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
};
