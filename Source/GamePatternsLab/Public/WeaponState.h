// Copyright © 2026 Yuii551. 

#pragma once

class APatternLabCharacter;

class FWeaponState
{
public:
	virtual ~FWeaponState() = default;

	virtual void Fire(APatternLabCharacter& Character) const = 0;
	virtual void Reload(APatternLabCharacter& Chararcter) const = 0;
	virtual void FinishCooldown(APatternLabCharacter& Chararcter) const {};
	virtual void FinishReload(APatternLabCharacter& Character) const {};
};

class FReadyWeaponState final : public FWeaponState
{
public:
	virtual void Fire(APatternLabCharacter& Character) const override;
	virtual void Reload(APatternLabCharacter& Character) const override;
};

class FCooldownWeaponState final : public FWeaponState
{
public:
	virtual void Fire(APatternLabCharacter& Character) const override;
	virtual void Reload(APatternLabCharacter& Character) const override;
	virtual void FinishCooldown(APatternLabCharacter& Character) const override;
};

class FEmptyWeaponState final : public FWeaponState
{
public:
	virtual void Fire(APatternLabCharacter& Character) const override;
	virtual void Reload(APatternLabCharacter& Character) const override;
};

class FReloadingWeaponState final : public FWeaponState
{
public:
	virtual void Fire(APatternLabCharacter& Character) const override;
	virtual void Reload(APatternLabCharacter& Character) const override;
	virtual void FinishReload(APatternLabCharacter& Character) const override;
};