// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class NNEngineDemoTarget : TargetRules
{
	public NNEngineDemoTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "NNEngineDemo", "WebcamFacialCapture", "WebcamMotionCapture" } );
	}
}
