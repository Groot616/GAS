// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	// Attribute에 있는 Intelligence 값을 캡쳐하기 위한 코드
	IntDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	// 적용 대상 관련 설정
	IntDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// Snapshot 사용 여부
	IntDef.bSnapshot = false;

	// Modifier Magnitude Calculation에 필요한 변수인 RelavantAttributesToCapture 배열에 IntelligenceDef 추가
	RelevantAttributesToCapture.Add(IntDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Source와 Target에서 태그 수집
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// FAggregatorEvaluateParameters : GameplayEffectd 계산 과정에서 Attribute값의 평가를 제어하기 위한 다양한 파리미터 소유
	// 해당 코드에서느 수집한 태그 할당
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.SourceTags = TargetTags;

	// 임시로 Intelligence값 할당
	float Int = 0.f;
	GetCapturedAttributeMagnitude(IntDef, Spec, EvaluationParameters, Int);
	Int = FMath::Max<float>(Int, 0.f);

	// ICombatInterface의 GetPlayerLevel() 호출을 위한 코드
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 50.f + 2.5f * Int + 15.f * PlayerLevel;
}
