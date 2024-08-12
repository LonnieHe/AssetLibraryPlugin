// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetUtil.h"
#include "HttpServerResponse.h"
#include "WebUtil.h"
/**
 * 
 */
class ASSETLIBRARYPLUGIN_API ServerUtil
{
public:

	static bool Start(uint32);
	static void Stop();

	static FHttpRequestHandler CreateHandler(const UnrealHttpServer::FHttpResponser& HttpResponser);
	static FString GetJsonValue(const FString& JsonString, const FString& Key);
	static TUniquePtr<FHttpServerResponse> FindInfo(const FHttpServerRequest& Request);
	static TUniquePtr<FHttpServerResponse> Response(const FAssetInfo& AssetInfo);
	static TUniquePtr<FHttpServerResponse> Response(TArray<uint8> BinaryData);
	static TUniquePtr<FHttpServerResponse> PathResponse();
};
