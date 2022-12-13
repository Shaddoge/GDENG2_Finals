// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MyEnumPrimitive.h"
#include "MyPrimitive.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GDENG2_UE_PARSER_API AMyPrimitive : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
		EnumPrimitiveType primitiveType;
};
