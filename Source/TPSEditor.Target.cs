// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using System;

public class TPSEditorTarget : TargetRules
{
    public TPSEditorTarget(TargetInfo Target) : base(Target)
    {
        Console.WriteLine("TPS editor target ---------------->");

        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("TPS");
    }
}
