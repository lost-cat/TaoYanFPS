﻿using UnrealBuildTool;

public class SLGModule : ModuleRules
{
    public SLGModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "EnhancedInput","UMG","AIModule","NavigationSystem", "CommonUI",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", "Niagara",
                
                
            }
        );
    }
}