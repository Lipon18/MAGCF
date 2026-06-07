// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MAGCFEditorTarget : TargetRules
{
    public MAGCFEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;

        ExtraModuleNames.AddRange(new string[] { "MAGCF" });
    }
}
