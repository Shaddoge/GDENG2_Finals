// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPrimitive.h"
#include "UObject/NoExportTypes.h"
#include "Parser.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)

class GDENG2_UE_PARSER_API UParser : public UObject
{
	GENERATED_BODY()
public:
	UParser();
	~UParser();
public:
	UFUNCTION(BlueprintCallable) TArray<AMyPrimitive*> Load(FString fileName);
	UFUNCTION(BlueprintCallable) void Save(FString fileName, TArray<AMyPrimitive*> myPrimitives);
private:
	FString CreateStringJsonObject(FString key);
	FString CreateArrayFromVector(FVector vector);
public:
	UPROPERTY(BlueprintReadOnly) FString objName;
};
