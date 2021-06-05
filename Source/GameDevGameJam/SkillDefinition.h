// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "SkillDefinition.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEVGAMEJAM_API USkillDefinition : public UItemDefinition
{
	GENERATED_BODY()
	
public:
	USkillDefinition()
	{
		ItemType = UGASAssetManager::SkillItemType;
	}

};
