// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved

using System.IO;
using UnrealBuildTool;

public class WebcamFacialCapture : ModuleRules
{
	public WebcamFacialCapture(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnableExceptions = true;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"OnnxRuntimeNNEngine",
				"CustomizedOpenCV",
				"TextureProcessing"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Linux)
		{
			RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "BlazeFaceDetection_1x256x256xBGRxByte_opset12.onnx"));
			RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "BlazeFaceLandmark_attention_1x192x192xBGRxByte_opset12.onnx"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "BlazeFaceDetection_1x256x256xBGRxByte_opset12.basic.ort"));
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "BlazeFaceLandmark_attention_1x192x192xBGRxByte_opset12.basic.ort"));
        }
	}
}
