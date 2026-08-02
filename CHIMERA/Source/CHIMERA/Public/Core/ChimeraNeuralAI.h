// GDD 14.19 — Advanced NPC AI System. Neural network-driven cognition with:
// 5-layer AI stack, Maslow needs hierarchy, 7-axis personality, 3-tier memory.
// ONNX-compatible inference pipeline ready for trained models.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraNeuralAI.generated.h"

// === GDD 14.19.1 — Needs Hierarchy (Maslow) ===
UENUM()
enum class EAI_Need : uint8 { Survival, Physiological, Safety, Social, Esteem, SelfActualization, COUNT };

USTRUCT()
struct FAI_NeedState
{
	GENERATED_BODY()
	float Values[6] = { 50.f, 50.f, 50.f, 50.f, 50.f, 10.f }; // 0..100, higher = more urgent
	bool bInCrisis = false; // emergency override: all needs suspended
	float GetUrgency(EAI_Need N) const { return bInCrisis ? 100.f : Values[(int32)N]; }
};

// === GDD 14.19.1 — 7-Axis Personality Vector ===
USTRUCT()
struct FAI_Personality
{
	GENERATED_BODY()
	float Openness = 50.f;         // Traditional(0) ↔ Curious(100)
	float Conscientiousness = 50.f; // Messy(0) ↔ Organized(100)
	float Extraversion = 50.f;     // Solitary(0) ↔ Social(100)
	float Agreeableness = 50.f;    // Hostile(0) ↔ Compassionate(100)
	float Neuroticism = 50.f;      // Calm(0) ↔ Anxious(100)
	float Ambition = 50.f;         // Content(0) ↔ Driven(100)
	float Morality = 50.f;         // Sociopath(0) ↔ Saintly(100)

	// Personality-derived behavioral weights
	float GetTrustThreshold() const { return Agreeableness * 0.5f + Neuroticism * -0.3f + 30.f; }
	float GetFightThreshold() const { return 80.f - (Neuroticism * 0.4f + Ambition * 0.3f); }
	float GetFleeThreshold() const { return 20.f + Neuroticism * 0.5f; }
	float GetSocialDesire() const { return Extraversion * 0.6f + Openness * 0.4f; }
};

// === GDD 14.19.1 — 3-Tier Memory System ===
USTRUCT()
struct FAI_Memory
{
	GENERATED_BODY()
	// Short-term: immediate environment (last 60 seconds, ~16 slots)
	TArray<FString> ShortTerm;
	float ShortTermTimer = 0.f;

	// Episodic: significant events (500 slots, remembered for days/weeks)
	TArray<FString> Episodic;

	// Semantic: learned knowledge (unlimited, permanent)
	TMap<FString, float> Semantic; // concept → confidence level

	void RememberShortTerm(const FString& What) { ShortTerm.Insert(What, 0); if (ShortTerm.Num() > 16) ShortTerm.Pop(); }
	void RememberEpisodic(const FString& What) { Episodic.Insert(What, 0); if (Episodic.Num() > 500) Episodic.Pop(); }
	void LearnSemantic(const FString& Concept, float Confidence) { Semantic.FindOrAdd(Concept) = FMath::Max(Semantic.FindRef(Concept), Confidence); }
};

// === Neural Decision: output of the inference pipeline ===
USTRUCT()
struct FAI_Decision
{
	GENERATED_BODY()
	FString Action;       // "flee", "fight", "talk", "trade", "sleep", "eat", "work", "socialize", "wander"
	FString Target;       // NPC/player name or location
	float Confidence = 0.f;
	int32 Priority = 0;   // 0=low, 3=urgent
	FString DialogueLine; // what they'd say if spoken to
};

// === GDD 14.19.2 — Dialogue Layer weights per personality ===
namespace ChimeraAIDialogue
{
	static FString PersonalityGreeting(const FAI_Personality& P)
	{
		if (P.Agreeableness > 70) return P.Extraversion > 60 ? TEXT("Oh, hello! Lovely day, isn't it?") : TEXT("Hello. How can I help you?");
		if (P.Neuroticism > 70) return TEXT("What? What do you want? I'm busy.");
		if (P.Ambition > 70) return TEXT("I don't have time. I'm on a deadline.");
		return P.Extraversion > 50 ? TEXT("Hey there.") : TEXT("Hm.");
	}

	static FString CrisisDialogue(const FAI_NeedState& N, const FAI_Personality& P)
	{
		if (N.bInCrisis) return P.Neuroticism > 60 ? TEXT("WE'RE ALL GOING TO DIE!") : TEXT("Get to safety. Now.");
		if (N.Values[(int32)EAI_Need::Survival] > 80) return TEXT("I need to get out of here.");
		if (N.Values[(int32)EAI_Need::Physiological] > 80) return TEXT("I haven't eaten in days...");
		return TEXT("Everything's fine.");
	}
}

// === Main Neural AI Subsystem ===
UCLASS()
class CHIMERA_API UNeuralAISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// === GDD 14.19 — Neural inference pipeline ===
	// Core decision: takes current state, personality, needs, memory → produces action
	FAI_Decision Infer(const FString& NpcName, const FAI_NeedState& Needs, const FAI_Personality& Personality, const FAI_Memory& Memory, const FString& EnvironmentState);

	// === Pre-trained behavior model (stub for ONNX) ===
	// In production: this loads a trained neural network from disk.
	//  Input layer: needs(6) + personality(7) + memory_encoding(32) + environment(16) = 61 floats
	//  Hidden layers: 128 → 64 → 32 with ReLU
	//  Output layer: action_scores(12) + confidence + priority
	void LoadNeuralModel(const FString& ModelPath);
	bool bModelLoaded = false;

	// === Learning: NPCs adapt to player behavior ===
	void RecordPlayerInteraction(const FString& NpcName, const FString& Action, float Outcome);
	float GetLearnedTrust(const FString& NpcName) const;

	// === GDD 14.19.3/4 — Environmental interaction & neural skill learning ===
	USTRUCT()
	struct FAISkillState
	{
		GENERATED_BODY()
		float Accuracy = 15.f;         // shooting/combat precision (0..100)
		float ReactionTime = 80.f;     // lower=faster, 0=instant
		float CoverUsage = 20.f;       // how well they use environment for cover
		float DrivingSkill = 25.f;     // vehicle handling
		float MedicalSkill = 30.f;     // surgery/healing
		float NegotiationSkill = 35.f; // trade/dialogue
		float SocialAwareness = 45.f;  // lie detection, empathy
		float LearningRate = 0.5f;     // personality-derived, 0=slow 1=fast
		int32 PracticeCount = 0;
	};

	FAISkillState* GetSkills(const FString& NpcName);
	void RecordPractice(const FString& NpcName, const FString& Skill, float Outcome);
	float GetSkillLevel(const FString& NpcName, const FString& Skill) const;
	FString AssessEnvironment(const FString& Ring, const FString& SubZone, float DangerLevel) const;

private:
	// Simple weight matrix as fallback when ONNX model not available
	TArray<float> InferFallback(const FAI_NeedState& N, const FAI_Personality& P, const FAI_Memory& M) const;

	// Learned player reputation — NPCs remember how you treated them
	TMap<FString, float> LearnedTrustValues; // NpcName → -100(hates) to +100(loves)
	TMap<FString, int32> InteractionCounts;
};
