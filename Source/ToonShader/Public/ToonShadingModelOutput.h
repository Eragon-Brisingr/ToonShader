// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Materials/MaterialExpressionCustomOutput.h"
#include "ToonShadingModelOutput.generated.h"

/* 
*
*/
UCLASS()
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
	FString GetDisplayName() const override { return TEXT("ToonShading"); }
};
