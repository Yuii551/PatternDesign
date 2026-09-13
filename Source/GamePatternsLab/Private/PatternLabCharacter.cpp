// Copyright © 2026 Yuii551. Licensed under the MIT License.


#include "PatternLabCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

APatternLabCharacter::APatternLabCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.3f;

	FirstPersonCamera =
		CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(-10.0f, 0.0f, 64.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;

	JumpButtonCommand = MakeUnique<FJumpCharacterCommand>();
	FireButtonCommand = MakeUnique<FFireCharacterCommand>();
}

void APatternLabCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	const APlayerController* PlayerController =
		Cast<APlayerController>(GetController());

	if (!PlayerController)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

	if (!LocalPlayer)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!InputSubsystem || !DefaultMappingContext)
	{
		return;
	}

	InputSubsystem->RemoveMappingContext(DefaultMappingContext);
	InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
}

void APatternLabCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!ensureMsgf(
		EnhancedInput,
		TEXT("PatternLabCharacter requires Enhanced Input.")
	))
	{
		return;
	}

	if (MoveAction)
	{
		EnhancedInput->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&APatternLabCharacter::Move
		);
	}
	
	if (LookAction)
	{
		EnhancedInput->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&APatternLabCharacter::Look
		);
	}

	if (JumpAction)
	{
		EnhancedInput->BindAction(
			JumpAction,
			ETriggerEvent::Started,
			this,
			&APatternLabCharacter::ExecuteJumpButtonCommand
		);
		
		EnhancedInput->BindAction(
			JumpAction,
			ETriggerEvent::Completed,
			this,
			&ACharacter::StopJumping
		);
	}

	if (FireAction)
	{
		EnhancedInput->BindAction(
			FireAction,
			ETriggerEvent::Started,
			this,
			&APatternLabCharacter::ExecuteFireButtonCommand
		);

		EnhancedInput->BindAction(
			FireAction,
			ETriggerEvent::Completed,
			this,
			&ACharacter::StopJumping
		);
	}

	if (SwapCommandsAction)
	{
		EnhancedInput->BindAction(
			SwapCommandsAction,
			ETriggerEvent::Started,
			this,
			&APatternLabCharacter::SwapInputCommand
		);
	}

	if (ReloadAction)
	{
		EnhancedInput->BindAction(
			ReloadAction,
			ETriggerEvent::Started,
			this,
			&APatternLabCharacter::ReloadWeapon
		);
	}
}

void APatternLabCharacter::Move(const FInputActionValue& InputValue)
{
	const FVector2D Movement = InputValue.Get<FVector2D>();

	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector Forward =
		FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector Right =
		FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, Movement.Y);
	AddMovementInput(Right, Movement.X);
}

void APatternLabCharacter::Look(const FInputActionValue& InputValue)
{
	const FVector2D LookInput = InputValue.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void APatternLabCharacter::PerformFire()
{
	switch (WeaponState)
	{
	case ENaiveWeaponState::Ready:
		break;

	case ENaiveWeaponState::Cooldown:
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				1.0f,
				FColor::Yellow,
				TEXT("Weapon is cooling down.")
			);
		}
		return;

	case ENaiveWeaponState::Empty:
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				1.0f,
				FColor::Red,
				TEXT("Weapon is empty. Press R to reload.")
			);
		}
		return;
	}

	FireShot();

	CurrentAmmo--;

	if (CurrentAmmo <= 0)
	{
		CurrentAmmo = 0;
		WeaponState = ENaiveWeaponState::Empty;
	}
	else
	{
		WeaponState = ENaiveWeaponState::Cooldown;

		GetWorldTimerManager().SetTimer(
			FireCooldownTimer,
			this,
			&APatternLabCharacter::FinishFireCooldown,
			FireCooldown,
			false
		);
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.0f,
			FColor::Cyan,
			FString::Printf(
				TEXT("Ammo: %d / %d"),
				CurrentAmmo,
				MagazineSize
			)
		);
	}
}

void APatternLabCharacter::FireShot()
{
	if (!FirstPersonCamera)
	{
		return;
	}

	const FVector TraceStart =
		FirstPersonCamera->GetComponentLocation();

	const FVector ShotDirection =
		FirstPersonCamera->GetForwardVector();

	const FVector TraceEnd =
		TraceStart + ShotDirection * ShotDistance;

	FCollisionQueryParams QueryParams(
		SCENE_QUERY_STAT(PatternLabFire),
		true,
		this
	);

	FHitResult HitResult;

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	const FVector DebugEnd = bHit
		? HitResult.ImpactPoint
		: TraceEnd;

	DrawDebugLine(
		GetWorld(),
		TraceStart,
		DebugEnd,
		bHit ? FColor::Green : FColor::Red,
		false,
		1.0f,
		0,
		1.5f
	);

	if (!bHit || !HitResult.GetActor())
	{
		return;
	}

	UGameplayStatics::ApplyPointDamage(
		HitResult.GetActor(),
		DamagePerShot,
		ShotDirection,
		HitResult,
		GetController(),
		this,
		UDamageType::StaticClass()
	);
}

void APatternLabCharacter::ExecuteJumpButtonCommand()
{
	if (JumpButtonCommand)
	{
		JumpButtonCommand->Execute(*this);
	}
}

void APatternLabCharacter::ExecuteFireButtonCommand()
{
	if (FireButtonCommand)
	{
		FireButtonCommand->Execute(*this);
	}
}

void APatternLabCharacter::SwapInputCommand()
{
	Swap(JumpButtonCommand, FireButtonCommand);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Yellow,
			TEXT("Space and left mouse commands swapped.")
		);
	}

}

void APatternLabCharacter::ReloadWeapon()
{
	GetWorldTimerManager().ClearTimer(FireCooldownTimer);

	CurrentAmmo = MagazineSize;
	WeaponState = ENaiveWeaponState::Ready;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.0f,
			FColor::Green,
			TEXT("Weapon reloaded.")
		);
	}
}

void APatternLabCharacter::FinishFireCooldown()
{
	if (WeaponState == ENaiveWeaponState::Cooldown)
	{
		WeaponState = ENaiveWeaponState::Ready;
	}
}
