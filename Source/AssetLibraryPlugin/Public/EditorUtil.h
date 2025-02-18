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
	void PictureToMaterialARD(const FString& AssetName, const FString& MidPath, const FString& Albedo, const FString& Normal, const FString& ARD);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PictureToMaterial(const FString& AssetName, const FString& MidPath, const FString& Albedo, const FString& Normal, const FString& AO, const FString& Roughness, const FString& Height);

	UFUNCTION(BlueprintCallable , Category = "AssetLibrary")
	static UTexture2D* CreateTexture2DAsset(UTexture2D* Texture, FString Name = "Texture", bool FlipGreenChannel = false, bool VirtualTextureStreaming  = false);

	UFUNCTION(BlueprintCallable , Category = "AssetLibrary")
	static UTexture2D* MyRenderTargetCreateStaticTexture2DEditorOnly(UTextureRenderTarget2D* RenderTarget, FString InName, enum TextureCompressionSettings CompressionSettings, enum TextureMipGenSettings MipSettings, UTexture2D* Tex);

	static void CopyTexture(UTexture2D* SourceTexture, UTexture2D* TargetTexture);
};
