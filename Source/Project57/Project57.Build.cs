// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Project57 : ModuleRules
{
	public Project57(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] {"Project57"});
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AnimGraphRuntime",
        "NavigationSystem", "AIModule", "UMG",
        "OnlineSubsystem", "OnlineSubsystemUtils"});

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "OnlineSubsystemSteam" });
		
		// Uncomment if you are using online features
		//PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
