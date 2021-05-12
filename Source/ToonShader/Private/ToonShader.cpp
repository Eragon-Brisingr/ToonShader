// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToonShader.h"
#include <Interfaces/IPluginManager.h>

#define LOCTEXT_NAMESPACE "FToonShaderModule"

void FToonShaderModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// 注意模块的加载时机为"LoadingPhase": "PostConfigInit"，否则打包会报Can't map virtual shader source path
	const FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("ToonShader"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/ToonShader"), PluginShaderDir);
}

void FToonShaderModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToonShaderModule, ToonShader)