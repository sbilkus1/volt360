// GDD 5 (character & progression) + 6.1/6.2 (combat & stealth). The player pawn:
// movement, interaction, contextual combat, and the funnel for skills/currencies/
// quests/minigames into the session subsystem.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "GameFramework/Character.h"
#include "ChimeraCharacter.generated.h"

class AChimeraInteractable;
class UChimeraSessionSubsystem;
class UChimeraMinigame;

UCLASS()
class CHIMERA_API AChimeraCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AChimeraCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- GDD 5.1 character creation / progression ---
	void FinishCharacterCreation(const FString& LifePath);
	int32 GetAttribute(EAttribute A) const { return Attributes[(int32)A]; }
	void AddAttribute(EAttribute A, int32 Delta);
	void AddSkillXPByName(FName Id, int32 XP);
	void OnMinigameMistake();
	void HandleMinigameResult(EMinigameKind Kind, bool bSuccess, int32 Score);
	UChimeraSessionSubsystem* GetSession() const { return Session; }

	// --- GDD 6.1 combat ---
	void DealMeleeDamage();
	void DealRangedDamage();
	UPROPERTY() TObjectPtr<AChimeraInteractable> Focus;
	bool bStealth = false;

	// GDD 6.9 - dragon flight.
	bool bDragonFlying = false;
	void ToggleDragonFlight();

	// GDD 6.2 - non-lethal stealth takedown vs Focus.
	void StealthTakedown();

	// --- input state ---
	bool bSprint = false;

protected:
	// GDD 3.1 moment-to-moment loop input.
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void StartJump() { Jump(); }
	void StopJump() { StopJumping(); }
	void StartSprint() { bSprint = true; }
	void StopSprint() { bSprint = false; }
	void ToggleCrouch();
	void Interact();
	void MeleeAttack() { DealMeleeDamage(); }
	void RangedAttack() { DealRangedDamage(); }

	UPROPERTY(VisibleAnywhere, Category = "CHIMERA|Camera")
	TObjectPtr<class USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "CHIMERA|Camera")
	TObjectPtr<class UCameraComponent> Camera;

private:
	TArray<int32> Attributes;
	TObjectPtr<UChimeraSessionSubsystem> Session;
	float AttackCooldown = 0.f;
	float InteractionScan = 0.f;
	int32 MeleeCombo = 0;

	// Enhanced Input (created at runtime; no assets required).
	class UInputMappingContext* IMC;
	class UInputAction* IA_Move;
	class UInputAction* IA_Look;
	class UInputAction* IA_Jump;
	class UInputAction* IA_Sprint;
	class UInputAction* IA_Crouch;
	class UInputAction* IA_Interact;
	class UInputAction* IA_Melee;
	class UInputAction* IA_Ranged;
	class UInputAction* IA_Dragon;
	class UInputAction* IA_Takedown;
};
