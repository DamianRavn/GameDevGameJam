// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameDevGameJamCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GASAbilitySystemComponent.h"
#include "GASAttributeSet.h"
#include "GASGameplayAbility.h"
#include <GameplayEffectTypes.h>
#include "Engine/AssetManager.h"

//////////////////////////////////////////////////////////////////////////
// AGameDevGameJamCharacter

AGameDevGameJamCharacter::AGameDevGameJamCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//Asset Data
	//Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));

	//GAS
	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UGASAttributeSet>(TEXT("Attributes"));
}

// void AGameDevGameJamCharacter::BeginPlay() 
// {
// 	if (CurrentWeaponDefinition)
// 	{
// 		EquipWeapon(CurrentWeaponDefinition);
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("CurrentWeapon is null"));
// 	}

// 	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Sword"));
// 	Super::BeginPlay();
// }

// void AGameDevGameJamCharacter::EquipWeapon(const UWeaponDefinition* WeaponDef) 
// {
// 	UStaticMesh* mesh = WeaponDef->Mesh.LoadSynchronous();
// 	Weapon->SetStaticMesh(mesh);
// 	CurrentWeaponDefinition = WeaponDef;
// }

// void AGameDevGameJamCharacter::EquipWeaponFromID(const FPrimaryAssetId& id) 
// {
// 	// Get the global Asset Manager
//     UAssetManager& AssetManager = UAssetManager::Get();
// 	TSharedPtr<FStreamableHandle> Handle = AssetManager.LoadPrimaryAsset(id);
// 	if (Handle)
// 	{
// 		Handle->WaitUntilComplete(5, false);
// 	}

// 	UWeaponDefinition* WeaponDef = AssetManager.GetPrimaryAssetObject<UWeaponDefinition>(id);
    
// 	if (WeaponDef)
// 	{
// 		EquipWeapon(WeaponDef);
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("The Weapon was not loaded"));
// 	}
// }

class UAbilitySystemComponent* AGameDevGameJamCharacter::GetAbilitySystemComponent() const 
{
	return AbilitySystemComponent;
}

void AGameDevGameJamCharacter::InitializeAttributes() 
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
		
	}
	
}

void AGameDevGameJamCharacter::GiveAbilities() 
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGASGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
		
	}
	
}

void AGameDevGameJamCharacter::PossessedBy(AController* NewController) 
{
	Super::PossessedBy(NewController);

	//Server GAS Init
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	GiveAbilities();
}

void AGameDevGameJamCharacter::OnRep_PlayerState() 
{
	Super::OnRep_PlayerState();

	InitializeAttributes();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		if (InputComponent)
		{
			const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));

			AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
		}
		
	}
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGameDevGameJamCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGameDevGameJamCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGameDevGameJamCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGameDevGameJamCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGameDevGameJamCharacter::LookUpAtRate);

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}


void AGameDevGameJamCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGameDevGameJamCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGameDevGameJamCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGameDevGameJamCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
