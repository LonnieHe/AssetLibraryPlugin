// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerUtil.h"

#include "HttpServerModule.h"
#include "IHttpRouter.h"
#include "WebUtil.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"



bool ServerUtil::Start(const uint32 Port)
{
	auto HttpServerInstance = &FHttpServerModule::Get();
	UE_LOG(LogTemp, Log, TEXT("Starting AssetLibraryHttpServer Server..."));
	
	TSharedPtr<IHttpRouter> HttpRouter = HttpServerInstance->GetHttpRouter(Port);

	
	if (!HttpRouter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Listener Port:%d ..."),Port);
		return false;
	}

	FHttpPath InfoPath("/Info");
	FHttpPath ThumbnailPath("/ThumbnailPath");
	
	UE_LOG(LogTemp, Log, TEXT("Starting Bind Router..."));
	HttpRouter->BindRoute(InfoPath, EHttpServerRequestVerbs::VERB_POST, CreateHandler(FindInfo));
	HttpRouter->BindRoute(ThumbnailPath, EHttpServerRequestVerbs::VERB_POST, CreateHandler(FindThumbnail));

	HttpServerInstance->StartAllListeners();
	
	return true;
}

void ServerUtil::Stop()
{
	auto HttpServerModule = &FHttpServerModule::Get();
	HttpServerModule->StopAllListeners();
}

TUniquePtr<FHttpServerResponse> ServerUtil::JsonResponse(const FAssetInfo& AssetInfo)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	
	TArray< TSharedPtr<FJsonValue> > DependenciesJson;
	for( FName Dependency : AssetInfo.AssetDependencies)
	{
		DependenciesJson.Add(MakeShareable(new FJsonValueString(Dependency.ToString())));
	}
	
	JsonObject->SetStringField(TEXT("Class"), AssetInfo.AssetClass.ToString());
	JsonObject->SetArrayField(TEXT("Dependencies"), DependenciesJson);
	
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return FHttpServerResponse::Create(JsonString, TEXT("application/json"));
}


TUniquePtr<FHttpServerResponse> ServerUtil::BinaryResponse(TArray<uint8> BinaryData)
{
	return FHttpServerResponse::Create(BinaryData, TEXT("image/png"));
}


FHttpRequestHandler ServerUtil::CreateHandler(const UnrealHttpServer::FHttpResponser& HttpResponser)
{
	return [HttpResponser](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
	{
		auto Response = HttpResponser(Request);
		if (Response == nullptr)
		{
			return false;
		}
		OnComplete(MoveTemp(Response)); 
		return true;
	};
}

TUniquePtr<FHttpServerResponse> ServerUtil::FindInfo(const FHttpServerRequest& Request)
{
	UE_LOG(LogTemp, Log, TEXT("Asset Library Request Received, Processing..."));
	/* DebugLog */
	/* 
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Request Received, Processing..."));
	}

	for (auto QueryParam : Request.QueryParams)
	{
		UE_LOG(LogTemp, Log, TEXT("QueryParam: %s : %s"), *QueryParam.Key, *QueryParam.Value);
	}

	for (auto PathParam : Request.PathParams)
	{
		UE_LOG(LogTemp, Log, TEXT("PathParam: %s : %s"), *PathParam.Key, *PathParam.Value);
	} */
	
	FString RequestAsFString = UTF8_TO_TCHAR(reinterpret_cast<const char*>(Request.Body.GetData()));
	
	// UE_LOG(LogTemp, Log, TEXT("Body: %s"), *RequestAsFString);

	const FString Type = GetJsonValue(RequestAsFString,"Type");
	const FString PackagePath = GetJsonValue(RequestAsFString,"PackagePath");
	
	if(Type == "Info")
	{
		return JsonResponse(AssetUtil::GetInfo(PackagePath));
	}
	else if(Type == "Thumbnail")
	{
		return BinaryResponse(AssetUtil::GetThumbnail(PackagePath));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Request Type: %s"), *Type);
		return nullptr;
	}
}


// TUniquePtr<FHttpServerResponse> ServerUtil::FindThumbnail(const FHttpServerRequest& Request) 
// {
// 	UE_LOG(LogTemp, Log, TEXT("Request Received, Processing..."));
// 	if (GEngine != nullptr)
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Request Received, Processing..."));
// 	}
// 	FString RequestAsFString;
// 	for (auto QueryParam : Request.QueryParams)
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("QueryParam: %s : %s"), *QueryParam.Key, *QueryParam.Value);
// 		RequestAsFString = QueryParam.Value;
// 	}
//
// 	for (auto PathParam : Request.PathParams)
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("PathParam: %s : %s"), *PathParam.Key, *PathParam.Value);
// 	}
//
// 	// FString RequestAsFString = UTF8_TO_TCHAR(reinterpret_cast<const char*>(Request.Body.GetData()));
// 	
// 	UE_LOG(LogTemp, Log, TEXT("Body: %s"), *RequestAsFString);
//
// 	return BinaryResponse(AssetUtil::GetThumbnail(RequestAsFString));
// }

FString ServerUtil::GetJsonValue(const FString& JsonString, const FString& Key)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	FJsonSerializer::Deserialize(Reader, JsonObject);

	return JsonObject->GetStringField(Key);
}

