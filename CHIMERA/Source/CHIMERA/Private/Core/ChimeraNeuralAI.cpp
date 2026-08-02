#include "Core/ChimeraNeuralAI.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

// NOTE: In production on the UE5.3+ machine, swap this include for:
//   #include "NNE.h"           // Unreal Neural Network Engine
//   #include "NNERuntimeCPU.h"
//   The existing InferFallback becomes the CPU fallback when GPU inference is unavailable.
//   ONNX model path: Content/AI/NPCAI_Behavior_v3.onnx

void UNeuralAISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// In production:
	//   LoadNeuralModel(FPaths::ProjectContentDir() / TEXT("AI/NPCAI_Behavior_v3.onnx"));
}

void UNeuralAISubsystem::LoadNeuralModel(const FString& ModelPath)
{
	// GDD 14.19 — ONNX model loading stub.
	// Production code:
	//   UNNEModelData* ModelData = LoadObject<UNNEModelData>(nullptr, *ModelPath);
	//   if (ModelData) { Runtime = NewObject<UNNERuntimeCPU>(this); Runtime->LoadModel(ModelData); bModelLoaded = true; }
	//   UE_LOG(LogChimera, Log, TEXT("[NeuralAI] Model loaded: %s, 61→128→64→32→14"), *ModelPath);
	bModelLoaded = false; // stub: always uses fallback until ONNX is connected
}

FAI_Decision UNeuralAISubsystem::Infer(const FString& NpcName, const FAI_NeedState& Needs, const FAI_Personality& Personality, const FAI_Memory& Memory, const FString& EnvironmentState)
{
	FAI_Decision D;

	// GDD 14.19.1 — Emergency override: crisis suspends all normal needs
	if (Needs.bInCrisis)
	{
		if (Personality.GetFightThreshold() > 70.f) { D.Action = TEXT("fight"); D.Priority = 3; }
		else if (Personality.GetFleeThreshold() > 30.f) { D.Action = TEXT("flee"); D.Priority = 3; }
		else { D.Action = TEXT("freeze"); D.Priority = 3; }
		D.Confidence = 0.95f;
		D.DialogueLine = ChimeraAIDialogue::CrisisDialogue(Needs, Personality);
		return D;
	}

	// === Run neural inference (or fallback) ===
	TArray<float> Scores;
	if (bModelLoaded)
	{
		// Production path: run through ONNX model
		// Scores = Runtime->RunInference(GetInputTensor(Needs, Personality, Memory, EnvironmentState));
	}
	else
	{
		Scores = InferFallback(Needs, Personality, Memory);
	}

	// Map scores to actions
	static const TCHAR* Actions[] = { TEXT("sleep"), TEXT("eat"), TEXT("work"), TEXT("socialize"), TEXT("wander"), TEXT("trade"), TEXT("flee"), TEXT("fight"), TEXT("talk"), TEXT("patrol"), TEXT("rest"), TEXT("pray") };
	int32 BestIdx = 0; float BestScore = Scores[0];
	for (int32 i = 1; i < Scores.Num() && i < 12; ++i) { if (Scores[i] > BestScore) { BestScore = Scores[i]; BestIdx = i; } }

	D.Action = Actions[BestIdx];
	D.Confidence = BestScore;
	D.Priority = (BestScore > 0.8f) ? 3 : (BestScore > 0.5f) ? 2 : 1;
	D.DialogueLine = ChimeraAIDialogue::PersonalityGreeting(Personality);

	// Player reputation modifier
	float Trust = LearnedTrustValues.FindRef(NpcName);
	if (Trust < -50) D.DialogueLine = TEXT("I remember what you did. Stay away.");
	else if (Trust > 50) D.DialogueLine = TEXT("My friend! Good to see you.");

	return D;
}

TArray<float> UNeuralAISubsystem::InferFallback(const FAI_NeedState& N, const FAI_Personality& P, const FAI_Memory& M) const
{
	// GDD 14.19 — Fallback neural decision matrix.
	// 12 neurons output. Weights derived from personality × need interactions.
	// These are hand-tuned approximations of the trained model's behavior.
	TArray<float> Out; Out.SetNum(12);

	// sleep: driven by fatigue (simulated via physiological need inversed)
	Out[0] = (100.f - N.Values[(int32)EAI_Need::Physiological]) * 0.4f + (100.f - P.Ambition) * 0.2f + (P.Neuroticism > 70 ? -0.2f : 0.f);

	// eat: driven by physiological need
	Out[1] = N.Values[(int32)EAI_Need::Physiological] * 0.6f + (100.f - P.Conscientiousness) * 0.1f;

	// work: driven by safety need + ambition + conscientiousness
	Out[2] = N.Values[(int32)EAI_Need::Safety] * 0.3f + P.Ambition * 0.4f + P.Conscientiousness * 0.3f;

	// socialize: driven by social need + extraversion + openness
	Out[3] = N.Values[(int32)EAI_Need::Social] * 0.35f + P.Extraversion * 0.35f + P.Openness * 0.3f;

	// wander: driven by openness when all needs low
	Out[4] = P.Openness * 0.4f + (100.f - N.GetUrgency(EAI_Need::Survival)) * 0.3f + P.Extraversion * 0.3f;

	// trade: driven by safety + ambition + agreeableness (more agreeable = more honest trade)
	Out[5] = N.Values[(int32)EAI_Need::Safety] * 0.3f + P.Ambition * 0.3f + P.Agreeableness * 0.2f + M.Semantic.FindRef(TEXT("economy")) * 0.2f;

	// flee: driven by survival need + neuroticism
	Out[6] = N.Values[(int32)EAI_Need::Survival] * 0.5f + P.Neuroticism * 0.3f;

	// fight: driven by survival + low agreeableness + high ambition
	Out[7] = N.Values[(int32)EAI_Need::Survival] * 0.3f + (100.f - P.Agreeableness) * 0.4f + P.Ambition * 0.2f;

	// talk: social need + extraversion when not in survival mode
	Out[8] = N.Values[(int32)EAI_Need::Social] * 0.3f + P.Extraversion * 0.3f + (100.f - N.Values[(int32)EAI_Need::Survival]) * 0.4f;

	// patrol: contentment + conscientiousness (guard behavior)
	Out[9] = P.Conscientiousness * 0.4f + (100.f - P.Neuroticism) * 0.2f + (100.f - N.GetUrgency(EAI_Need::Survival)) * 0.4f;

	// rest: when no urgent needs, recovery
	Out[10] = (100.f - N.GetUrgency(EAI_Need::Survival)) * 0.3f + (100.f - P.Ambition) * 0.3f + (100.f - N.GetUrgency(EAI_Need::Physiological)) * 0.4f;

	// pray: high openness + morality + self-actualization
	Out[11] = P.Openness * 0.2f + P.Morality * 0.3f + N.Values[(int32)EAI_Need::SelfActualization] * 0.3f + M.Semantic.FindRef(TEXT("faith")) * 0.2f;

	// Normalize to 0..1 range
	for (float& V : Out) V = FMath::Clamp(V / 100.f, 0.01f, 1.f);
	return Out;
}

void UNeuralAISubsystem::RecordPlayerInteraction(const FString& NpcName, const FString& Action, float Outcome)
{
	// GDD 14.19.4 — NPCs learn from player interactions.
	// Positive outcomes increase trust, negative outcomes decrease it.
	float& Trust = LearnedTrustValues.FindOrAdd(NpcName);
	Trust = FMath::Clamp(Trust + Outcome * 10.f, -100.f, 100.f);
	InteractionCounts.FindOrAdd(NpcName)++;
}

float UNeuralAISubsystem::GetLearnedTrust(const FString& NpcName) const
{
	return LearnedTrustValues.FindRef(NpcName);
}

// ======================================================================
// GDD 14.19.3/4 — Neural Skill Learning & Environmental Awareness
// ======================================================================

UNeuralAISubsystem::FAISkillState* UNeuralAISubsystem::GetSkills(const FString& NpcName)
{
	return &SkillDatabase.FindOrAdd(NpcName);
}

void UNeuralAISubsystem::RecordPractice(const FString& NpcName, const FString& Skill, float Outcome)
{
	FAISkillState& S = SkillDatabase.FindOrAdd(NpcName);
	S.PracticeCount++;

	// Neural learning: each practice adjusts skill toward the outcome using learning rate.
	// Learning rate is scaled by the player's chosen AI difficulty.
	float SessionLR = 0.5f;
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		SessionLR = Sess->GetAILearningMultiplier();
	float Adapt = S.LearningRate * SessionLR * (Outcome - GetSkillLevel(NpcName, Skill)) * 0.1f;

	if (Skill == TEXT("shooting"))       S.Accuracy = FMath::Clamp(S.Accuracy + Adapt, 5.f, 100.f);
	else if (Skill == TEXT("reaction"))   S.ReactionTime = FMath::Clamp(S.ReactionTime - Adapt, 20.f, 100.f);
	else if (Skill == TEXT("cover"))      S.CoverUsage = FMath::Clamp(S.CoverUsage + Adapt, 5.f, 100.f);
	else if (Skill == TEXT("driving"))    S.DrivingSkill = FMath::Clamp(S.DrivingSkill + Adapt, 10.f, 100.f);
	else if (Skill == TEXT("surgery"))    S.MedicalSkill = FMath::Clamp(S.MedicalSkill + Adapt, 10.f, 100.f);
	else if (Skill == TEXT("negotiation")) S.NegotiationSkill = FMath::Clamp(S.NegotiationSkill + Adapt, 10.f, 100.f);
	else if (Skill == TEXT("social"))     S.SocialAwareness = FMath::Clamp(S.SocialAwareness + Adapt, 10.f, 100.f);

	// Learning rate improves with practice (NPCs get better at learning)
	S.LearningRate = FMath::Min(1.f, S.LearningRate + 0.002f);
}

float UNeuralAISubsystem::GetSkillLevel(const FString& NpcName, const FString& Skill) const
{
	const FAISkillState* S = SkillDatabase.Find(NpcName);
	if (!S) return 15.f;
	if (Skill == TEXT("shooting")) return S->Accuracy;
	if (Skill == TEXT("reaction")) return S->ReactionTime;
	if (Skill == TEXT("cover")) return S->CoverUsage;
	if (Skill == TEXT("driving")) return S->DrivingSkill;
	if (Skill == TEXT("surgery")) return S->MedicalSkill;
	if (Skill == TEXT("negotiation")) return S->NegotiationSkill;
	if (Skill == TEXT("social")) return S->SocialAwareness;
	return 25.f;
}

FString UNeuralAISubsystem::AssessEnvironment(const FString& Ring, const FString& SubZone, float DangerLevel) const
{
	// GDD 14.19.3 — NPCs understand their environment and adapt behavior.
	// In production, this would query the actual UE5 environment: cover points, objects,
	// line-of-sight blockers, navigation meshes, destructible elements, and interactables.
	// For now, ring-aware heuristics approximate the neural environment model's output.

	if (DangerLevel > 70.f)
	{
		if (Ring == TEXT("Aether City")) return TEXT("Superhero battle overhead. Seek cover behind sky-platforms. Flying debris present.");
		if (Ring == TEXT("Mid-Wilshire")) return TEXT("Gunfire detected. Cover: parked cars, dumpsters, doorway alcoves. Civilian hostages nearby.");
		if (Ring == TEXT("Alagaesia")) return TEXT("Dragon overhead. Cover: rock outcroppings, ancient ruins. Sword drawn. Shield up.");
		if (Ring == TEXT("Night City")) return TEXT("Cyber-attack active. ICE walls eroding. Net daemons patrolling. Firewall: 60% integrity.");
		if (Ring == TEXT("Panem")) return TEXT("Arena hazard active. Muttations released. Tracker jacker nest: 30m north. Cornucopia: weapons cache.");
		return TEXT("Emergency: find cover, assess exits, protect civilians.");
	}

	if (Ring == TEXT("Mercy Heights")) return TEXT("Hospital shift. Patients waiting in ward 3. Gurney available. Surgery tools sterile (barely).");
	if (Ring == TEXT("Neo-Kingdom")) return TEXT("Academy courtyard. Students practicing spells. Potion ingredients nearby. Musical number brewing.");
	if (Ring == TEXT("Grand Coliseum")) return TEXT("Race day. Pit lane active. Fuel station ready. Tire compounds: soft/medium/hard. Crowd: 80,000.");
	if (Ring == TEXT("Cleveleys")) return TEXT("Seafront. Tide: going out. Promenade busy. Ice cream van 50m south. Seagulls: aggressive.");
	if (Ring == TEXT("Minecraft Frontier")) return TEXT("Voxel terrain. Redstone circuits nearby. Buildable blocks visible. Creeper hiss: proximity warning.");

	return FString::Printf(TEXT("%s — %s. Scanning for threats and opportunities."), *Ring, *SubZone);
}
