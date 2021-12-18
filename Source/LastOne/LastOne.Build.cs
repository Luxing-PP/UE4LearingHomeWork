// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LastOne : ModuleRules
{
	public LastOne(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","AIModule", "GameplayTasks","HeadMountedDisplay", "UMG","RHI", "RenderCore" });
	}
}
