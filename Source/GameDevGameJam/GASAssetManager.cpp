// Fill out your copyright notice in the Description page of Project Settings.


#include "GASAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "ItemDefinition.h"

const FPrimaryAssetType	UGASAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType	UGASAssetManager::SkillItemType = TEXT("Skill");
const FPrimaryAssetType	UGASAssetManager::WeaponItemType = TEXT("Weapon");
const FPrimaryAssetType	UGASAssetManager::TokenItemType = TEXT("Token");

UGASAssetManager& UGASAssetManager::Get()
{
	UGASAssetManager* This = Cast<UGASAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid AssetManager in DefaultEngine.ini, must be UGASAssetManager!"));
		return *NewObject<UGASAssetManager>(); // never calls this
	}
}

void UGASAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}


UItemDefinition* UGASAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{	
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UItemDefinition* LoadedItem = Cast<UItemDefinition>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}

