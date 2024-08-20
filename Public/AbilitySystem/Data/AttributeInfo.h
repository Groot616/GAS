// Copyright Groot

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

// AttributeInfo 저장을 위한 구조체
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
    GENERATED_BODY()

    // 어떤 Attribute의 info인지 태그를 통해 지정
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag AttributeTag = FGameplayTag();

    // AttributeMenu 위젯에서 사용할 Attribute명을 지정
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText AttributeName = FText();

    // 추가 설명용
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText Description = FText();

    // Attribute의 값
    UPROPERTY(BlueprintReadOnly)
    float AttributeValue = 0.f;
};

/**
 * 
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
    // 태그에 맞는 Attribute를 찾는 함수
    FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

    // Attribute의 정보를 저장할 구조체배열
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FAuraAttributeInfo> AttributeInformation;
};
