// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CatchUp : ModuleRules
{
	public CatchUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore" 
		});

		PublicIncludePaths.AddRange(new string[]
		{
			"CatchUp/",
			"CatchUp/Public/Components/Game/",
			"CatchUp/Public/Settings/Game/",
			"CatchUp/Public/Player/Game/",
			"CatchUp/Public/Items/Game/",
			"CatchUp/Public/UI/Game/"
		});
	}
}
