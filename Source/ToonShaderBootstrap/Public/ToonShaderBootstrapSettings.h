// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ToonShaderBootstrapSettings.generated.h"

/**
 * 
 */
UCLASS(Config = "ToonShaderSettings", defaultconfig)
class TOONSHADERBOOTSTRAP_API UToonShaderBootstrapSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UToonShaderBootstrapSettings()
		: bAlwaysReplaceShaderDirectory(false)
		, bRestoreDefaultShaderDirectory(true)
	{}
	
	FName GetContainerName() const override { return TEXT("Project"); }
	FName GetCategoryName() const override { return TEXT("Plugins"); }
	FName GetSectionName() const override { return TEXT("ToonShaderSettings"); }

	// 每次启动时都会将卡通材质shader替换进引擎的shader文件中
	// 建议修改shader调试时才打开
	UPROPERTY(EditAnywhere, Config, Category = "启动设置", meta = (DisplayName = "总是替换ToonShader文件夹"))
	uint8 bAlwaysReplaceShaderDirectory : 1;

	// 关闭项目时会恢复为虚幻默认的Shader
	// 可能会导致引擎启动时shader编译
	UPROPERTY(EditAnywhere, Config, Category = "启动设置", meta = (DisplayName = "关闭项目时恢复默认Shader"))
	uint8 bRestoreDefaultShaderDirectory : 1;
};
