#pragma once
#include "CoreMinimal.h"
#include "EffectTypes.generated.h"

UENUM(BlueprintType)
enum class EEffectType : uint8
{
    Instant     UMETA(DisplayName = "Instant"),
    Buff        UMETA(DisplayName = "Buff"),
    Debuff      UMETA(DisplayName = "Debuff"),
};

UENUM(BlueprintType)
enum class EEffectDurationType : uint8
{
    Instant      UMETA(DisplayName = "Instant"),      
    TimeBased    UMETA(DisplayName = "Time Based"),
    TurnBased    UMETA(DisplayName = "Turn Based"),
    Permanent    UMETA(DisplayName = "Permanent"),
};

// Cible de l'effet (ce qu'il modifie) à modifier pour votre projet
UENUM(BlueprintType)
enum class EStatTarget : uint8
{
    Health      UMETA(DisplayName = "Health"),
    MaxHealth   UMETA(DisplayName = "Max Health"),
    Critic       UMETA(DisplayName = "Critic"),
    Damage      UMETA(DisplayName = "Damage"),
    Defense     UMETA(DisplayName = "Defense"),
    Pe          UMETA(DisplayName = "PE"),
    Element     UMETA(DisplayName = "Element"),
};

UENUM(BlueprintType)
enum class EModifierOp : uint8
{
    Add         UMETA(DisplayName = "Add"),        // +10
    Multiply    UMETA(DisplayName = "Multiply"),   // x1.5
    Override    UMETA(DisplayName = "Override"),   // = 100
};

// -----------------------------------------------
// La donnée pure d'un effet — à créer en Blueprint
// -----------------------------------------------
USTRUCT(BlueprintType)
struct BASICEFFECTSYSTEM_API FEffectData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    FName EffectID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    EEffectType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    EStatTarget StatTarget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    EModifierOp ModifierOp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    float Value = 0.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Duration")
    EEffectDurationType DurationType = EEffectDurationType::Instant;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Duration",meta=(EditCondition="DurationType==EEffectDurationType::TimeBased", EditConditionHides))
    float Duration = 0.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Duration",meta=(EditCondition="DurationType==EEffectDurationType::TurnBased", EditConditionHides))
    int32 TurnCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    int32 MaxStacks = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    UTexture2D* Icon = nullptr;
};

USTRUCT(BlueprintType)
struct BASICEFFECTSYSTEM_API FActiveEffect
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FEffectData Data;
    UPROPERTY(BlueprintReadOnly) FGuid InstanceID;
    UPROPERTY(BlueprintReadOnly) int32 CurrentStacks = 1;

    // Time Based
    UPROPERTY(BlueprintReadOnly) float RemainingTime = 0.f;
    // Turn Based
    UPROPERTY(BlueprintReadOnly) int32 RemainingTurns = 0;
};