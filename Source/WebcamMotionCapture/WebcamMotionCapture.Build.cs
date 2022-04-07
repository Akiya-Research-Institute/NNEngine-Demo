// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved

using System.IO;
using UnrealBuildTool;

public class WebcamMotionCapture : ModuleRules
{
	public WebcamMotionCapture(ReadOnlyTargetRules Target) : base(Target)
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
			RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "BlazePoseDetection_1x128x128xBGRxByte_opset12.onnx"));
			RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "movenet_singlepose_lightning_4_1x192x192x3xBGRxByte_opset12.onnx"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "BlazePoseDetection_1x128x128xBGRxByte_opset12.basic.ort"));
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "movenet_singlepose_lightning_4_1x192x192x3xBGRxByte_opset12.basic.ort"));
        }
	}
}
