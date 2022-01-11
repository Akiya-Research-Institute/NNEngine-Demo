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
				"OnnxRuntime",
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

		RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "BlazePoseDetection_1x128x128xBGRxByte.onnx"));
		RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "movenet_lightning_v4_1x192x192xBGRxByte.onnx"));
	}
}
