#pragma once
#include "CoreMinimal.h"
#include "EffectTypes.h"
#include "UObject/Interface.h"
#include "IStatable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UStatable : public UInterface { GENERATED_BODY() };

class BASICEFFECTSYSTEM_API IStatable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetStatValue(EStatTarget Stat) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ModifyStat(EStatTarget Stat, EModifierOp Op, float Value);
};