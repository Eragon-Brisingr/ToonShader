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

#undef LOCTEXT_NAMESPACE