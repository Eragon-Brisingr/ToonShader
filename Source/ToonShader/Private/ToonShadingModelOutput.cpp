// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToonShadingModelOutput.h"

#if WITH_EDITOR
#include "MaterialCompiler.h"
#endif

#define LOCTEXT_NAMESPACE "ToonShadingModelOutput"

UToonShadingModelOutput::UToonShadingModelOutput(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	static FText NAME_Category = LOCTEXT("Toon", "Toon");
	MenuCategories.Add(NAME_Category);

	bCollapsed = true;

	// No outputs
	Outputs.Reset();
#endif
}

#if WITH_EDITOR
int32 UToonShadingModelOutput::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	auto CompileExpressionInput = [&](FExpressionInput& ExpressionInput, const FString& PinName)
	{
		if (ExpressionInput.GetTracedInput().Expression)
		{
			return Compiler->CustomOutput(this, OutputIndex, ExpressionInput.Compile(Compiler));
		}
		return CompilerError(Compiler, *FString::Printf(TEXT("%s Input missing"), *PinName));
	};
	
	switch (EToonShadingModelOutput(OutputIndex))
	{
	case EToonShadingModelOutput::SpecularRange:
		return CompileExpressionInput(SpecularRange, GET_MEMBER_NAME_STRING_CHECKED(UToonShadingModelOutput, SpecularRange));
	case EToonShadingModelOutput::Offset:
		return CompileExpressionInput(Offset, GET_MEMBER_NAME_STRING_CHECKED(UToonShadingModelOutput, Offset));
	}
	checkNoEntry();
	return CompilerError(Compiler, TEXT("Input missing"));
}

void UToonShadingModelOutput::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(FString(TEXT("ToonShading")));
}

uint32 UToonShadingModelOutput::GetInputType(int32 InputIndex)
{
	switch (EToonShadingModelOutput(InputIndex))
	{
	case EToonShadingModelOutput::SpecularRange:
		return MCT_Float1;
	case EToonShadingModelOutput::Offset:
		return MCT_Float1;
	}
	checkNoEntry();
	return MCT_Float1;
}

FExpressionInput* UToonShadingModelOutput::GetInput(int32 InputIndex)
{
	switch (EToonShadingModelOutput(InputIndex))
	{
	case EToonShadingModelOutput::SpecularRange:
		return &SpecularRange;
	case EToonShadingModelOutput::Offset:
		return &Offset;
	}
	checkNoEntry();
	return nullptr;
}
#endif

//#if WITH_EDITOR
//UToonMaterialShadingModelExpression::UToonMaterialShadingModelExpression(const FObjectInitializer& ObjectInitializer)
//{
//#if WITH_EDITORONLY_DATA
//
//	MenuCategories.Add(LOCTEXT("Shading Model", "Shading Model"));
//
//	// bShaderInputData = true;
//	bShowOutputNameOnPin = true;
//
//	Outputs.Reset();
//	Outputs.Add(FExpressionOutput(TEXT("")));
//#endif
//}
//
//int32 UToonMaterialShadingModelExpression::Compile(FMaterialCompiler* Compiler, int32 OutputIndex)
//{
//	return Compiler->ShadingModel(static_cast<EMaterialShadingModel>(ToonShadingModel));
//}
//
//uint32 UToonMaterialShadingModelExpression::GetOutputType(int32 OutputIndex)
//{
//	return MCT_ShadingModel;
//}
//
//void UToonMaterialShadingModelExpression::GetCaption(TArray<FString>& OutCaptions) const
//{
//	const UEnum* ShadingModelEnum = StaticEnum<EToonMaterialShadingModel>();
//	check(ShadingModelEnum);
//
//	const FString ShadingModelDisplayName = ShadingModelEnum->GetDisplayNameTextByValue(static_cast<uint8>(ToonShadingModel)).ToString();
//
//	// Add as a stack, last caption to be added will be the main (bold) caption
//	OutCaptions.Add(ShadingModelDisplayName);
//	OutCaptions.Add(TEXT("Toon Shading Model"));
//}
//#endif

#undef LOCTEXT_NAMESPACE
