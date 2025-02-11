#include "AssetUtil.h"

#include "EditorUtil.h"
#include "ObjectTools.h"
#include "IImageWrapperModule.h"

FString AssetUtil::PackageName2ObjectPath(const FString& PackageName)
{
	FString ObjectName;
	PackageName.Split(TEXT("/"), nullptr, &ObjectName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	return PackageName + "." + ObjectName;
}

FAssetInfo AssetUtil::GetInfo(const FString& PackageName)
{
	FString ObjectPath = PackageName2ObjectPath(PackageName);
	AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(ObjectPath);
	
	if(AssetData.GetAsset() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset not found: %s"), *PackageName);
		return FAssetInfo();
	}
	
	TArray<FName> AssetDependencies;
	AssetRegistryModule.Get().GetDependencies(AssetData.PackageName, AssetDependencies);

	FName AssetClass = AssetData.AssetClassPath.GetAssetName();

	return FAssetInfo(AssetDependencies, AssetClass);
}

TArray <uint8> AssetUtil::GetThumbnail(const FString& PackageName, QueryMode Mode)
{
	FString ObjectPath = PackageName2ObjectPath(PackageName);
	AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(ObjectPath);
	
	FObjectThumbnail* ObjectThumbnail = nullptr;
	FThumbnailMap ThumbnailMap;
	
	if(AssetData.GetAsset() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset not found: %s"), *PackageName);
		return TArray <uint8>();
	}

	if(Mode == QueryMode::Render)
	{
		ObjectThumbnail = ThumbnailTools::GenerateThumbnailForObjectToSaveToDisk(AssetData.GetAsset());
	}
	else if(Mode == QueryMode::Cache)
	{
		FString PackageFilename;
		FPackageName::DoesPackageExist(AssetData.PackageName.ToString(), &PackageFilename);
		const FName AssetFullName = FName(*AssetData.GetFullName());
		TSet<FName> AssetFullNames;
		AssetFullNames.Add(AssetFullName);
		
		ThumbnailTools::LoadThumbnailsFromPackage(PackageFilename, AssetFullNames, ThumbnailMap);
		ObjectThumbnail = ThumbnailMap.Find(AssetFullName);	
	}
	
	if (ObjectThumbnail)
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::Get().LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
		ImageWrapper->SetRaw(ObjectThumbnail->GetUncompressedImageData().GetData(), ObjectThumbnail->GetUncompressedImageData().Num(), ObjectThumbnail->GetImageWidth(), ObjectThumbnail->GetImageHeight(), ERGBFormat::BGRA, 8);
		if (ImageWrapper)
		{
			TArray64<uint8> CompressedByteArray = ImageWrapper->GetCompressed();
			TArray <uint8> ByteArray;
			ByteArray.Append(CompressedByteArray.GetData(), CompressedByteArray.Num());
			return  ByteArray;
		}
	}

	return TArray <uint8>();
	
}
// AO is ARD when bUseARD is true
bool AssetUtil::LoadAndExecuteBlueprint(const FString& AssetName,const FString& MidPath, const FString& Albedo, const FString& Normal, const FString& AO, const FString& Roughness, const FString& Height, bool bUseARD)
{
	UObject* BlueprintAsset = StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/AssetLibraryPlugin/EUB_PictureToMaterial/EUB_PicToMaterial.EUB_PicToMaterial"));
	if (UBlueprint* Blueprint = Cast<UBlueprint>(BlueprintAsset))
	{
		UObject* CDO = Blueprint->GeneratedClass->GetDefaultObject();
		if (UEditorUtil* BP_EUB = Cast<UEditorUtil>(CDO))
		{
			if (bUseARD)
			{
				BP_EUB->PictureToMaterialARD(AssetName, MidPath, Albedo, Normal, AO);
			}
			else
			{	
				BP_EUB->PictureToMaterial(AssetName, MidPath, Albedo, Normal, AO, Roughness, Height);
			}
		}
	}
	// need process error
	return true;
}

bool AssetUtil::PicToMaterial(const FString& AssetName, const FString& MidPath, const FString& Albedo, const FString& Normal, const FString& ARD)
{
	return LoadAndExecuteBlueprint(AssetName, MidPath, Albedo, Normal, ARD, FString(), FString(), true);
}

bool AssetUtil::PicToMaterial(const FString& AssetName, const FString& MidPath, const FString& Albedo, const FString& Normal, const FString& AO, const FString& Roughness, const FString& Height)
{
	return LoadAndExecuteBlueprint(AssetName, MidPath, Albedo, Normal, AO, Roughness, Height, false);
}


