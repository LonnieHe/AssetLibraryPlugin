#include "AssetUtil.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "ObjectTools.h"
#include "IImageWrapperModule.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/FileHelper.h"

FAssetInfo AssetUtil::GetInfo(const FString& ObjectPath)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	
	FString Path = "/Game" + ObjectPath;
	FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(Path);//may soft object path

	TArray<FName> AssetDependencies;
	AssetRegistryModule.Get().GetDependencies(AssetData.PackageName, AssetDependencies);

	FName AssetClass = AssetData.AssetClassPath.GetAssetName();

	return FAssetInfo(AssetDependencies, AssetClass);
}

TArray <uint8> AssetUtil::GetThumbnail(const FString& ObjectPath)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	
	FString Path = "/Game" + ObjectPath;
	FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(Path);


	// FObjectThumbnail* ObjectThumbnail = ThumbnailTools::GenerateThumbnailForObjectToSaveToDisk(AssetData.GetAsset());
	FObjectThumbnail* ObjectThumbnail = ThumbnailTools::GenerateThumbnailForObjectToSaveToDisk(AssetData.GetAsset());
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
