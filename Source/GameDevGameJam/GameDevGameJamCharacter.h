// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "WeaponDefinition.h"
#include "GameDevGameJamCharacter.generated.h"

UCLASS(config=Game)
class AGameDevGameJamCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	class UGASAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UGASAttributeSet* Attributes;

public:
	AGameDevGameJamCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	// class UStaticMeshComponent* Weapon;

	//Data Asset
	//static const UWeaponDefinition WeaponDefinition;
	// void EquipWeaponFromID(const FPrimaryAssetId& id);
	// void EquipWeapon(const UWeaponDefinition* WeaponDef);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DataAsset)
	const UWeaponDefinition* CurrentWeaponDefinition;

	//GAS
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAttributes();
	virtual void GiveAbilities();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//Effect that initializes default values
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	//Effect that initializes default values
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<class UGASGameplayAbility>> DefaultAbilities;

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
};

