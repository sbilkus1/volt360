// GDD 10 - Technical Architecture. Game target.
using UnrealBuildTool;
using System.Collections.Generic;

public class CHIMERATarget : TargetRules
{
	public CHIMERATarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("CHIMERA");
	}
}
