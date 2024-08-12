// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerUtil.h"

#include "HttpServerModule.h"
#include "IHttpRouter.h"
#include "WebUtil.h"
#include "Kismet/KismetSystemLibrary.h"
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

	UE_LOG(LogTemp, Log, TEXT("Starting Bind Router..."));
	
	FHttpPath InfoPath("/Info");
	HttpRouter->BindRoute(InfoPath, EHttpServerRequestVerbs::VERB_POST, CreateHandler(FindInfo));

	HttpServerInstance->StartAllListeners();
	
	return true;
}

void ServerUtil::Stop()
{
	auto HttpServerModule = &FHttpServerModule::Get();
	HttpServerModule->StopAllListeners();
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
	
	FString RequestAsFString = UTF8_TO_TCHAR(reinterpret_cast<const char*>(Request.Body.GetData()));
	const FString Type = GetJsonValue(RequestAsFString,"Type");
	const FString PackagePath = GetJsonValue(RequestAsFString,"PackagePath");
	
	if(Type == "Info")
	{
		return Response(AssetUtil::GetInfo(PackagePath));
	}
	else if(Type == "Thumbnail")
	{
		return Response(AssetUtil::GetThumbnail(PackagePath));
	}
	else if(Type == "Path") 
	{
		return PathResponse();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Request Type: %s"), *Type);
	}
	
	return nullptr;
}


FString ServerUtil::GetJsonValue(const FString& JsonString, const FString& Key)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	FJsonSerializer::Deserialize(Reader, JsonObject);

	return JsonObject->GetStringField(Key);
}

// If you want to get other information(s).
// Please create/reload/edit the following response and call from FindInfo().

// Dependencies response
TUniquePtr<FHttpServerResponse> ServerUtil::Response(const FAssetInfo& AssetInfo)
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
// Thumbnail response
TUniquePtr<FHttpServerResponse> ServerUtil::Response(TArray<uint8> BinaryData)
{
	return FHttpServerResponse::Create(BinaryData, TEXT("image/png"));
}
// Project root path response
TUniquePtr<FHttpServerResponse> ServerUtil::PathResponse()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("Path"), UKismetSystemLibrary::GetProjectDirectory());
	
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return FHttpServerResponse::Create(JsonString, TEXT("application/json"));
}