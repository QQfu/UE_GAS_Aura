// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Data/AuraCharacterClassInfoAsset.h"

FCharacterClassInfo UAuraCharacterClassInfoAsset::GetCharacterClassInfo(ECharacterClass CharacterClass) const
{
	return CharacterClassDefaultMap.FindChecked(CharacterClass);
}
