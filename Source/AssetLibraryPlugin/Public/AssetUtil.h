#pragma once
#include "CoreMinimal.h"

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
	//static void SaveThumbnail(FString ObjectPath, FString OutputPath);
	
	static FAssetInfo GetInfo(const FString& ObjectPath);
	static TArray <uint8> GetThumbnail(const FString& ObjectPath);
};
