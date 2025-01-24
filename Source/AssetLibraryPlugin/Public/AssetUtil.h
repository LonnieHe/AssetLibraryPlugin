#pragma once
#include "CoreMinimal.h"
#include "AssetRegistry/AssetRegistryModule.h"



struct FAssetInfo
{
	
	TArray<FName> AssetDependencies;

	FName AssetClass;

	FAssetInfo()
	{
		AssetDependencies = TArray<FName>();
		AssetClass = FName();
	}
	
	FAssetInfo(const TArray<FName>& Dependencies, const FName& Class)
	{
		AssetDependencies = Dependencies;
		AssetClass = Class;
	}
};

namespace AssetUtil
{
	enum QueryMode
	{
		Cache = 0,
		Render
	};
	
	static FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	static FAssetData AssetData;
	
	static FString PackageName2ObjectPath(const FString& PackageName);
	
	static FAssetInfo GetInfo(const FString& PackageName);
	static TArray <uint8> GetThumbnail(const FString& PackageName, QueryMode Mode);

	static bool PicToMaterial(const FString& AssetName, const FString& PackagePath, const FString& FilePath);
};
