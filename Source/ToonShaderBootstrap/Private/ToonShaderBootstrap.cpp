// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToonShaderBootstrap.h"
#include <Interfaces/IPluginManager.h>
#include <HAL/PlatformFilemanager.h>

#define LOCTEXT_NAMESPACE "FToonShaderBootstrapModule"

void FToonShaderBootstrapModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	check(GConfig);

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	const FString PluginShaderOverrideDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("ToonShader"))->GetBaseDir(), TEXT("ShadersOverride"));
	const FString ShaderDir = FPaths::Combine(FPaths::EngineDir(), TEXT("Shaders"));

	bool bAlwaysReplaceShaderDirectory = PlatformFile.DirectoryExists(*FPaths::Combine(ShaderDir, TEXT("Private"), TEXT("Toon"))) == false;
	if (bAlwaysReplaceShaderDirectory == false)
	{
		FConfigFile* ConfigFile = GConfig->Find(*FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("DefaultToonShaderSettings.ini")), false);
		if (ConfigFile)
		{
			ConfigFile->GetBool(TEXT("/Script/ToonShaderBootstrap.ToonShaderBootstrapSettings"), TEXT("bAlwaysReplaceShaderDirectory"), bAlwaysReplaceShaderDirectory);
		}
	}

	if (bAlwaysReplaceShaderDirectory)
	{
		PlatformFile.CopyDirectoryTree(*ShaderDir, *FPaths::Combine(PluginShaderOverrideDir, TEXT("Override")), true);
	}
}

void FToonShaderBootstrapModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	check(GConfig);

	bool bRestoreDefaultShaderDirectory = true;
	{
		FConfigFile* ConfigFile = GConfig->Find(*FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("DefaultToonShaderSettings.ini")), false);
		if (ConfigFile)
		{
			ConfigFile->GetBool(TEXT("/Script/ToonShaderBootstrap.ToonShaderBootstrapSettings"), TEXT("bRestoreDefaultShaderDirectory"), bRestoreDefaultShaderDirectory);
		}
	}
	
	if (bRestoreDefaultShaderDirectory)
	{
		const FString PluginShaderOverrideDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("ToonShader"))->GetBaseDir(), TEXT("ShadersOverride"));
		const FString ShaderDir = FPaths::Combine(FPaths::EngineDir(), TEXT("Shaders"));
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		PlatformFile.DeleteDirectory(*FPaths::Combine(ShaderDir, TEXT("Private"), TEXT("Toon")));
		PlatformFile.CopyDirectoryTree(*ShaderDir, *FPaths::Combine(PluginShaderOverrideDir, TEXT("Default")), true);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToonShaderBootstrapModule, ToonShaderBootstrap)