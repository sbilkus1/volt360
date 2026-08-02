// GDD 10 - Technical Architecture. Editor target (editor + game builds).
using UnrealBuildTool;
using System.Collections.Generic;

public class CHIMERAEditorTarget : TargetRules
{
	public CHIMERAEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("CHIMERA");
	}
}
