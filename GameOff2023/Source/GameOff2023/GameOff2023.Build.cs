// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameOff2023 : ModuleRules
{
	public GameOff2023(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
