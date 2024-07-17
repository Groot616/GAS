// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// Attribute에 있는 Vigor 값을 캡쳐하기 위한 코드
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	// 적용 대상 관련 설정
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// Snapshot 사용 여부
	VigorDef.bSnapshot = false;

	// Modifier Magnicute Calcultaion에 필요한 변수인 RelevantAttributesToCapture 배열에 VigorDef 추가
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Source와 Target에서 태그 수집
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// FAggregatorEvaluateParameters : GameplayEffect 계산 과정에서 Attribute값의 평가를 제어하기 위한 다양한 파라미터 소유
	// 해당 코드에서는 수집한 태그 할당
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// 임시로 Vigor값 할당
	float Vigor = 0.f;
	// 특정 Attribute 크기를 평가하여 반환, 해당 코드에서는 Vigor 크기 반환
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	// ICombatInterface의 GetPlayerLevel() 호출을 위한 코드
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
