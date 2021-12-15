// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class NNEngineDemoEditorTarget : TargetRules
{
	public NNEngineDemoEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "NNEngineDemo", "WebcamFacialCapture", "WebcamMotionCapture" } );
	}
}
