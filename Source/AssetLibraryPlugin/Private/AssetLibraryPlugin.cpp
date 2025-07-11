// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetLibraryPlugin.h"
#include "EditorUtil.h"
#include "ServerUtil.h"

#define LOCTEXT_NAMESPACE "FAssetLibraryPluginModule"

void FAssetLibraryPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	ServerUtil::Start(7788);
}

void FAssetLibraryPluginModule::ShutdownModule()
{
	ServerUtil::Stop();
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssetLibraryPluginModule, AssetLibraryPlugin)
