// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using System;
using EpicGames.Core;

public class TPSTarget : TargetRules
{
    [CommandLine("-UnoptimizedCode")]
    public bool UnoptimizedCode = false;

    public TPSTarget(TargetInfo Target) : base(Target)
    {
        // Console.WriteLine("TPS game target ---------------->");
        // Console.WriteLine("TPSTarget UnoptimizedCode ----------------> {0}", UnoptimizedCode);
        if (UnoptimizedCode)
        {
            ProjectDefinitions.Add("UNOPTIMIZED_CODE");
        }

        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("TPS");
    }
}
