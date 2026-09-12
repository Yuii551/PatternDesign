// Copyright © 2026 Yuii551. Licensed under the MIT License.

#pragma once

class APatternLabCharacter;

class FCharacterCommand
{
public:
	virtual ~FCharacterCommand() = default;

	virtual void Execute(APatternLabCharacter& Character) const = 0;
};

class FJumpCharacterCommand final : public FCharacterCommand
{
public:
	virtual void Execute(
		APatternLabCharacter& Character) const override;
};

class FFireCharacterCommand final : public FCharacterCommand
{
public:
	virtual void Execute(
		APatternLabCharacter& Character) const override;
};