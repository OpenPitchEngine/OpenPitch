// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyExternalLibBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class OPENPITCH_UE_API UMyExternalLibBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category="MyExternalLib")
	static FString GetExternalMessage();
};
