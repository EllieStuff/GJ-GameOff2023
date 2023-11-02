// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GameOff2023Target : TargetRules
{
	public GameOff2023Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		//DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("GameOff2023");
	}
}
