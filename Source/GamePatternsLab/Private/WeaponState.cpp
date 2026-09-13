// Copyright © 2026 Yuii551. Licensed under the MIT License.

#include "WeaponState.h"

#include "PatternLabCharacter.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

namespace
{
	void ShowMessage(const FString& Message, const FColor& Color)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, Color, Message);
		}
	}
	
	void ShowAmmo(const int32 CurrentAmmo, const int32 MagazineSize)
	{
		ShowMessage(
			FString::Printf(
				TEXT("Ammo: %d / %d"),
				CurrentAmmo,
				MagazineSize
			),
			FColor::Cyan
		);
	}
}

void FReadyWeaponState::Fire(APatternLabCharacter& Character) const
{
	Character.FireShot();
	
	Character.CurrentAmmo =
		FMath::Max(Character.CurrentAmmo - 1, 0);

	ShowAmmo(Character.CurrentAmmo, Character.MagazineSize);

	if (Character.CurrentAmmo <= 0)
	{
		Character.CurrentWeaponState =
				&Character.EmptyWeaponState;

		return;
	}

	Character.CurrentWeaponState =
		&Character.CooldownWeaponState;

	Character.GetWorldTimerManager().SetTimer(
		Character.FireCooldownTimer,
		&Character,
		&APatternLabCharacter::FinishFireCooldown,
		Character.FireCooldown,
		false
	);
}

void FReadyWeaponState::Reload(APatternLabCharacter& Character) const
{
	Character.RefillWeapon();
}

void FReadyWeaponState::FinishCooldown(APatternLabCharacter& Character) const
{
	// no cooldown for ready state
}

void FCooldownWeaponState::Fire(APatternLabCharacter& Character) const
{
	ShowMessage(
		TEXT("Weapon is cooling down."),
		FColor::Yellow
	);
}

void FCooldownWeaponState::Reload(APatternLabCharacter& Character) const
{
	Character.RefillWeapon();
}

void FCooldownWeaponState::FinishCooldown(APatternLabCharacter& Character) const
{
	Character.CurrentWeaponState =
		&Character.ReadyWeaponState;
}

void FEmptyWeaponState::Fire(APatternLabCharacter& Character) const
{
	ShowMessage(
		TEXT("Weapon is empty. Press R to reload."),
		FColor::Red
	);
}

void FEmptyWeaponState::Reload(APatternLabCharacter& Character) const
{
	Character.RefillWeapon();
}

void FEmptyWeaponState::FinishCooldown(APatternLabCharacter& Character) const
{
	// no cooldown for empty state
}
