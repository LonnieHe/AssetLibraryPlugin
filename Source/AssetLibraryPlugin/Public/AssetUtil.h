#pragma once
#include "CoreMinimal.h"
#include "AssetRegistry/AssetRegistryModule.h"

/* AssetUtil.h
	Get asset data.
	Process Path and format.
	Call blueprint to create asset.
*/

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

	static bool PicToMaterial(const FString& AssetName, const FString& MidPath, const FString& Albedo, const FString& Normal, const FString& ARD);
	static bool PicToMaterial(const FString& AssetName, const FString& MidPath, const FString& Albedo, const FString& Normal, const FString& AO, const FString& Roughness, const FString& Height);
	static bool LoadAndExecuteBlueprint(const FString& AssetName, const FString& MidPath, const FString& Albedo, const FString& Normal, const FString& AO, const FString& Roughness, const FString& Height, bool bUseARD);
};
