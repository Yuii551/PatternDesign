// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatternLabCharacter.generated.h"

class UCameraComponent;

UCLASS()
class GAMEPATTERNSLAB_API APatternLabCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APatternLabCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
