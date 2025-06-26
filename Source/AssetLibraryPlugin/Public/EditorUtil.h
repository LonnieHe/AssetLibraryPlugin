// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "EditorUtil.generated.h"

/**
 * The code that truly handles asset creation
 */

UCLASS()
class ASSETLIBRARYPLUGIN_API UEditorUtil : public UAssetActionUtility
{
	GENERATED_BODY()
public:
	/*
	 * Create MATERIAL asset from downloaded image.
	 * Implement in BluePrint and Call by AssetUtil::PicToMaterial().
	 */
	UFUNCTION(BlueprintImplementableEvent)
	bool PictureToMaterialARD(const FString& AssetName, const FString& MidPath, const FString& Albedo, const FString& Normal, const FString& ARD);
	
	UFUNCTION(BlueprintImplementableEvent)
	bool PictureToMaterial(const FString& AssetName, const FString& MidPath, const FString& Albedo, const FString& Normal, const FString& AO, const FString& Roughness, const FString& Height);

	/*
	 * Create TEXTURE asset from downloaded image.
	 * Call this function (BluePrint) to create a new TEXTURE asset in the editor.
	 * @param Texture The source texture to copy from
	 * @param InName The name of the new texture asset
	 */
	UFUNCTION(BlueprintCallable , Category = "AssetLibrary")
	static UTexture2D* CreateTexture2DAsset(UTexture2D* Texture, FString InName = "Texture", bool FlipGreenChannel = false, bool VirtualTextureStreaming  = false, bool sRGB = false, int CompressionSettings = 0);

	/* 
	 * UNUSED. Another function to create texture from render target
	*/
	UFUNCTION(BlueprintCallable , Category = "AssetLibrary")
	static UTexture2D* MyRenderTargetCreateStaticTexture2DEditorOnly(UTextureRenderTarget2D* RenderTarget, FString InName, enum TextureCompressionSettings CompressionSettings, enum TextureMipGenSettings MipSettings, UTexture2D* Tex);

	static void CopyTexture(UTexture2D* SourceTexture, UTexture2D* TargetTexture);
};
