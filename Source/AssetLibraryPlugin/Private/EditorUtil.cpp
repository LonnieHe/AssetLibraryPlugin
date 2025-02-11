// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorUtil.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Texture2DDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Internationalization/Internationalization.h"


void UEditorUtil::CopyTexture(UTexture2D* SourceTexture, UTexture2D* TargetTexture)
{
	if (!SourceTexture || !TargetTexture)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Texture Pointer!"));
		return;
	}

	// 确保源纹理有数据
	FTexture2DMipMap& SourceMip = SourceTexture->PlatformData->Mips[0];
	if (SourceMip.BulkData.GetBulkDataSize() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Source texture has no valid data!"));
		return;
	}

	// 获取源纹理数据
	void* SourceData = SourceMip.BulkData.Lock(LOCK_READ_ONLY);
	if (!SourceData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to lock source texture data!"));
		return;
	}

	// 获取纹理格式
	EPixelFormat PixelFormat = SourceTexture->PlatformData->PixelFormat;
	int32 Width = SourceTexture->GetSizeX();
	int32 Height = SourceTexture->GetSizeY();

	
	// 创建目标纹理的数据
	TargetTexture->PlatformData = new FTexturePlatformData();
	TargetTexture->PlatformData->SizeX = Width;
	TargetTexture->PlatformData->SizeY = Height;
	TargetTexture->PlatformData->PixelFormat = PixelFormat;

	// 创建 MIP 数据
	FTexture2DMipMap* TargetMip = new FTexture2DMipMap();
	TargetMip->SizeX = Width;
	TargetMip->SizeY = Height;
	TargetTexture->PlatformData->Mips.Add(TargetMip);

	// 分配数据存储空间
	TargetMip->BulkData.Lock(LOCK_READ_WRITE);
	void* TargetData = TargetMip->BulkData.Realloc(SourceMip.BulkData.GetBulkDataSize());
	FMemory::Memcpy(TargetData, SourceData, SourceMip.BulkData.GetBulkDataSize());
	TargetMip->BulkData.Unlock();
	SourceMip.BulkData.Unlock();

	// 更新资源

	TargetTexture->Source.Init(Width, Height, 1, 1, ETextureSourceFormat::TSF_BGRA8, static_cast<const uint8*>(SourceData));
	TargetTexture->UpdateResource();
}



UTexture2D* UEditorUtil::CreateTexture2DAsset(UTexture2D* Texture, FString InName, bool FlipGreenChannel, bool VirtualTextureStreaming)
{
#if WITH_EDITOR
	if (!Texture)
	{
		FMessageLog("Blueprint").Warning(FText::FromString(TEXT("Input Texture null: Failed to create a new texture.")));
		return nullptr;
	}
	else
	{
		FString Name;
		FString PackageName;
		IAssetTools& AssetTools = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		//Use asset name only if directories are specified, otherwise full path
		InName.RemoveFromStart(TEXT("/"));
		InName.RemoveFromStart(TEXT("Content/"));
		InName.StartsWith(TEXT("Game/")) == true ? InName.InsertAt(0, TEXT("/")) : InName.InsertAt(0, TEXT("/Game/"));
		AssetTools.CreateUniqueAssetName(InName, TEXT(""), PackageName, Name);

		UObject* NewObj = nullptr;

		// create a static 2d texture
		UPackage* Package = CreatePackage(*PackageName);
		UTexture2D* Result = NewObject<UTexture2D>(Package, FName(*Name), RF_Public | RF_Standalone | RF_MarkAsRootSet);

		// Copy the texture data
		CopyTexture(Texture, Result);

		//Update the resource to apply changes
		if (Result != nullptr)
		{
			// package needs saving
			Package->MarkPackageDirty();
		
			// Update  settings
			Result->VirtualTextureStreaming = VirtualTextureStreaming;
			Result->bFlipGreenChannel = FlipGreenChannel;

			Result->PostEditChange();

			// Notify the asset registry
			FAssetRegistryModule::AssetCreated(Result);

			//???
			FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
			bool bSaved = UPackage::SavePackage(Package, Result, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

			return Result;
		}
		FMessageLog("Blueprint").Warning(FText::FromString(TEXT("CreateTexture2D_FailedToCreateTexture: Failed to create a new texture.")));
	}
#else
	FMessageLog("Blueprint").Warning(FText::FromString(TEXT("Texture2D's cannot be created at runtime.")));
#endif
	return nullptr;
}


UTexture2D* UEditorUtil::MyRenderTargetCreateStaticTexture2DEditorOnly(UTextureRenderTarget2D* RenderTarget, FString InName, enum TextureCompressionSettings CompressionSettings, enum TextureMipGenSettings MipSettings, UTexture2D* Tex)
{
#if WITH_EDITOR
	if (!RenderTarget)
	{
		FMessageLog("Blueprint").Warning(FText::FromString(TEXT("Input Texture null: Failed to create a new texture.")));
		return nullptr;
	}
	else if (!RenderTarget->GetResource())
	{
		FMessageLog("Blueprint").Warning(FText::FromString(TEXT("Input Texture null: Failed to create a new texture.")));
		return nullptr;
	}
	else
	{
		FString Name;
		FString PackageName;
		IAssetTools& AssetTools = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		//Use asset name only if directories are specified, otherwise full path
		if (!InName.Contains(TEXT("/")))
		{
			FString AssetName = RenderTarget->GetOutermost()->GetName();
			const FString SanitizedBasePackageName = UPackageTools::SanitizePackageName(AssetName);
			const FString PackagePath = FPackageName::GetLongPackagePath(SanitizedBasePackageName) + TEXT("/");
			AssetTools.CreateUniqueAssetName(PackagePath, InName, PackageName, Name);
		}
		else
		{
			InName.RemoveFromStart(TEXT("/"));
			InName.RemoveFromStart(TEXT("Content/"));
			InName.StartsWith(TEXT("Game/")) == true ? InName.InsertAt(0, TEXT("/")) : InName.InsertAt(0, TEXT("/Game/"));
			AssetTools.CreateUniqueAssetName(InName, TEXT(""), PackageName, Name);
		}

		UObject* NewObj = nullptr;

		// create a static 2d texture
		// NewObj = RenderTarget->ConstructTexture2D(CreatePackage( *PackageName), Name, RenderTarget->GetMaskedFlags() | RF_Public | RF_Standalone, CTF_Default | CTF_AllowMips | CTF_SkipPostEdit, nullptr);
		UTexture2D* NewTex = Cast<UTexture2D>(NewObj);
		UPackage* Package = CreatePackage(*PackageName);
		UTexture2D* Result = NewObject<UTexture2D>(Package, FName(*Name), RF_Public | RF_Standalone | RF_MarkAsRootSet);
		CopyTexture(Tex, Result);

		
		// Result->PostEditChange();
		
		if (Result != nullptr)
		{
			// package needs saving
			Package->MarkPackageDirty();

			// Update Compression and Mip settings
			Result->CompressionSettings = CompressionSettings;
			Result->MipGenSettings = MipSettings;
			Result->VirtualTextureStreaming = true;
			Result->bFlipGreenChannel = true;
			Result->NeverStream = false;
			// NewTex->SourceFilePath_DEPRECATED = RenderTarget->GetPathName();
			Result->PostEditChange();

			// Notify the asset registry
			FAssetRegistryModule::AssetCreated(NewObj);

			//???
			FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
			bool bSaved = UPackage::SavePackage(Package, Result, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

			return Result;
		}
		FMessageLog("Blueprint").Warning(FText::FromString(TEXT("Input Texture null: Failed to create a new texture.")));
	}
#else
	FMessageLog("Blueprint").Error(LOCTEXT("Texture2D's cannot be created at runtime.", "RenderTargetCreateStaticTexture2DEditorOnly: Can't create Texture2D at run time. "));
#endif
	return nullptr;
}

