// Copyright © 2026 Yuii551. Licensed under the MIT License.

#include "CharacterCommand.h"

#include "PatternLabCharacter.h"

void FJumpCharacterCommand::Execute(APatternLabCharacter& Character) const
{
	Character.Jump();
}

void FFireCharacterCommand::Execute(APatternLabCharacter& Character) const
{
	Character.PerformFire();
}
