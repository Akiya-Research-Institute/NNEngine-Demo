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
				"OnnxRuntime",
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

		RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "BlazeFaceDetection_1x256x256xBGRxByte.onnx"));
		RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "BlazeFaceLandmark_1x192x192xBGRxByte.onnx"));
		RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "..", "ThirdParty", "Models", "BlazeIris_1x64x64xBGRxByte.onnx"));

	}
}
