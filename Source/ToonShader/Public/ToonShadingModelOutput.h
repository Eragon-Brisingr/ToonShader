// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Materials/MaterialExpressionCustomOutput.h"
#include "ToonShadingModelOutput.generated.h"

/* 
*
*/
UCLASS(collapsecategories, hidecategories = Object, MinimalAPI)
class UToonShadingModelOutput : public UMaterialExpressionCustomOutput
{
	GENERATED_BODY()
public:
	UToonShadingModelOutput(const FObjectInitializer& ObjectInitializer);

	enum class EToonShadingModelOutput
	{
		SpecularRange = 0,
		Offset = 1,
	};
	
	UPROPERTY(meta = (RequiredInput = "true"))
	FExpressionInput SpecularRange;

	UPROPERTY(meta = (RequiredInput = "true"))
	FExpressionInput Offset;

#if WITH_EDITOR
	int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	void GetCaption(TArray<FString>& OutCaptions) const override;
	uint32 GetInputType(int32 InputIndex) override;
	FExpressionInput* GetInput(int32 InputIndex) override;
#endif
	int32 GetNumOutputs() const override { return 2; }
	FString GetFunctionName() const override { return TEXT("GetToonShading"); }
	FString GetDisplayName() const override { return TEXT("Toon Shading"); }
};

// FMaterialShadingModelField::AddShadingModel 存在check(InShadingModel < MSM_NUM)检查导致没法Hack
//UENUM()
//enum class EToonMaterialShadingModel : uint8
//{
//	MSM_STYLIZED_SHADOW = 12		UMETA(DisplayName = "Style Shadow"),
//};
//
//UCLASS(collapsecategories, hidecategories = Object, MinimalAPI, meta = (DisplayName = "Toon Shading Model"))
//class UToonMaterialShadingModelExpression : public UMaterialExpression
//{
//	GENERATED_BODY()
//public:
//	UToonMaterialShadingModelExpression(const FObjectInitializer& ObjectInitializer);
//	
//	//~ Begin UMaterialExpression Interface
//#if WITH_EDITOR
//	int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
//	uint32 GetOutputType(int32 OutputIndex) override;
//	void GetCaption(TArray<FString>& OutCaptions) const override;
//#endif
//public:
//	UPROPERTY(EditAnywhere, Category = ShadingModel)
//	EToonMaterialShadingModel ToonShadingModel = EToonMaterialShadingModel::MSM_STYLIZED_SHADOW;
//	//~ End UMaterialExpression Interface
//};
