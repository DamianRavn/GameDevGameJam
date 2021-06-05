// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "WeaponDefinition.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEVGAMEJAM_API UWeaponDefinition : public UItemDefinition
{
	GENERATED_BODY()

public:
	UWeaponDefinition()
	{
		ItemType = UGASAssetManager::WeaponItemType;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AActor> WeaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta =(AssetBundles = "Game"))
	TArray<TSoftObjectPtr<UAnimMontage>> SwingAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta =(AssetBundles = "Game"))
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta =(AssetBundles = "Game"))
	float DamgeMultiplier;
};
