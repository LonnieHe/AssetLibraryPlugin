// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "EditorUtil.generated.h"

/**
 * 
 */
UCLASS()
class ASSETLIBRARYPLUGIN_API UEditorUtil : public UAssetActionUtility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void PictureToMaterialARD(const FString& AssetName, const FString& Albedo, const FString& Normal, const FString& ARD);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PictureToMaterial(const FString& AssetName, const FString& Albedo, const FString& Normal, const FString& AO, const FString& Roughness, const FString& Height);
};
