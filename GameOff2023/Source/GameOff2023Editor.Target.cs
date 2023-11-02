// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GameOff2023EditorTarget : TargetRules
{
	public GameOff2023EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		//DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("GameOff2023");
	}
}
