// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetUtil.h"
#include "HttpServerResponse.h"
#include "WebUtil.h"
/* ServerUtil.h
	Listening port 7788
	Processing messages
	Call functions in AssetUtil.h
	Return response
 */
class ASSETLIBRARYPLUGIN_API ServerUtil
{
public:

	static bool Start(uint32);
	static void Stop();

	static FHttpRequestHandler CreateHandler(const UnrealHttpServer::FHttpResponser& HttpResponser);
	static FString GetJsonValue(const FString& JsonString, const FString& Key);
	static TUniquePtr<FHttpServerResponse> GetAssetInfo(const FHttpServerRequest& Request);
	static TUniquePtr<FHttpServerResponse> GetAssetThumbnail(const FHttpServerRequest& Request);
	static TUniquePtr<FHttpServerResponse> GetAssetPath(const FHttpServerRequest&);
	static TUniquePtr<FHttpServerResponse> PicToMaterial(const FHttpServerRequest& Request);
	static TUniquePtr<FHttpServerResponse> Response(const FAssetInfo& AssetInfo);
	static TUniquePtr<FHttpServerResponse> Response(TArray<uint8> BinaryData);
	static TUniquePtr<FHttpServerResponse> PathResponse();
};
