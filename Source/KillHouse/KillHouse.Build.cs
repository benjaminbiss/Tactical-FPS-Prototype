// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KillHouse : ModuleRules
{
	public KillHouse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
            "AnimGraphRuntime"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"KillHouse",
			"KillHouse/Variant_Horror",
			"KillHouse/Variant_Horror/UI",
			"KillHouse/Variant_Shooter",
			"KillHouse/Variant_Shooter/AI",
			"KillHouse/Variant_Shooter/UI",
			"KillHouse/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
