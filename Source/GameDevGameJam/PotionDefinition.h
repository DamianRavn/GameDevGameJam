// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "PotionDefinition.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEVGAMEJAM_API UPotionDefinition : public UItemDefinition
{
	GENERATED_BODY()
	
public:
	UPotionDefinition()
	{
		ItemType = UGASAssetManager::PotionItemType;
	}
};
