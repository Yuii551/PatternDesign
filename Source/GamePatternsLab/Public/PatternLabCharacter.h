// Copyright © 2026 Yuii551. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "CharacterCommand.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "PatternLabCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ENaiveWeaponState : uint8
{
	Ready,
	Cooldown,
	Empty
	
};

UCLASS()
class GAMEPATTERNSLAB_API APatternLabCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APatternLabCharacter();

	void PerformFire();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float ShotDistance = 5000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float DamagePerShot = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UInputAction> SwapCommandsAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ReloadAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 MagazineSize = 3;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float FireCooldown = 0.5f;

	virtual void PawnClientRestart() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);

	TUniquePtr<FCharacterCommand> JumpButtonCommand;
	TUniquePtr<FCharacterCommand> FireButtonCommand;

	void ExecuteJumpButtonCommand();
	void ExecuteFireButtonCommand();
	void SwapInputCommand();

	ENaiveWeaponState WeaponState = ENaiveWeaponState::Ready;
	FTimerHandle FireCooldownTimer;

	void ReloadWeapon();
	void FinishFireCooldown();

	void FireShot();
};
