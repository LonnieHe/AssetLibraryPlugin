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
	void PictureToMaterial(const FString& AssetName,const FString& PackagePath,const FString& FilePath);
};
