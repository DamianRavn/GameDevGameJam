// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "TokenDefinition.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEVGAMEJAM_API UTokenDefinition : public UItemDefinition
{
	GENERATED_BODY()
	
public:
	UTokenDefinition()
	{
		ItemType = UGASAssetManager::TokenItemType;
	}
};