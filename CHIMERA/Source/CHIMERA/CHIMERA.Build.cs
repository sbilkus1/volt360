// GDD 10.1/10.2 - Engine: "Project Hydra" fork of UE7; prototype targets UE5 module set.
using UnrealBuildTool;

public class CHIMERA : ModuleRules
{
	public CHIMERA(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"Json",
			"Renderer"       // GDD 2 — path tracing / Lumen / Nanite
		});

		// GDD 2.1 — Path traced pipeline enable flags.
		// #define CHIMERA_RT_ENABLED 1       // uncomment after UE5 machine setup
		// Target.bEnableRayTracing = true;   // requires UE5.3+ and DX12

		// GDD 2.1/2.3 — Settings in DefaultEngine.ini [SystemSettings]:
		//   r.RayTracing=1
		//   r.RayTracing.Shadows=1
		//   r.RayTracing.Reflections=1
		//   r.RayTracing.GlobalIllumination=1
		//   r.Lumen.DiffuseIndirect.Allow=1
		//   r.Lumen.Reflections.Allow=1
		//   r.Nanite=1
		//   r.NRC.Enable=1
		//   r.NeuralMaterial.Enable=1
		// GDD 2.2 — Hardware targets: RTX 7090 (Ultra), RTX 6070 (High),
		//   RTX 5050 (Performance), AMD Z3 Extreme (Steam Deck 3)

		// GDD 7 — Multiplayer (stub for future server deployment):
		//   PrivateDependencyModuleNames.Add("OnlineSubsystem");
		//   PrivateDependencyModuleNames.Add("OnlineSubsystemEOS");

		PrivateDependencyModuleNames.AddRange(new string[] { });
}
