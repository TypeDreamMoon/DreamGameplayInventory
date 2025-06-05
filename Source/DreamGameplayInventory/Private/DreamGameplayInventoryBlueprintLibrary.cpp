// Copyright © Dream Moon Studio . Dream Moon All rights reserved


#include "DreamGameplayInventoryBlueprintLibrary.h"

#include "DreamGameplayInventoryInterface.h"
#include "Components/DreamInventoryComponent.h"
#include "Components/DreamInventorySlotComponent.h"
#include "Classes/DreamInventoryItem.h"


UDreamInventoryBase* UDreamGameplayInventoryBlueprintLibrary::GetDreamGameplayInventory(AActor* Target)
{
	if (!Target) return nullptr;
	
	if (IDreamGameplayInventoryInterface* Interface = Cast<IDreamGameplayInventoryInterface>(Target))
	{
		return Interface->Execute_GetInventory(Target);
	}
	else
	{
		return Target->GetComponentByClass<UDreamInventoryBase>();
	}
}

UDreamInventoryComponent* UDreamGameplayInventoryBlueprintLibrary::AsDreamGameplayInventoryComponent(UDreamInventoryBase* Inventory)
{
	if (!Inventory) return nullptr;

	return Cast<UDreamInventoryComponent>(Inventory);
}

UDreamInventorySlotComponent* UDreamGameplayInventoryBlueprintLibrary::AsDreamSlotInventoryComponent(UDreamInventoryBase* Inventory)
{
	if (!Inventory) return nullptr;

	return  Cast<UDreamInventorySlotComponent>(Inventory);
}

TArray<UDreamInventoryItem*> UDreamGameplayInventoryBlueprintLibrary::FilterItemByKeyWord(const TArray<UDreamInventoryItem*>& Target, FString KeyWord, ESearchCase::Type SearchCase)
{
	TArray<UDreamInventoryItem*> Result;
	
	auto DoesItemContainKeyword = [SearchCase](const UDreamInventoryItem* Item, FString InKeyWord)
	{
		if (!Item) return false;

		for (auto Element : Item->GetInformation().KeyWords)
		{
			if (Element.Contains(InKeyWord, SearchCase))
			{
				return true;
			}
			else
			{
				continue;
			}
		}

		return false;
	};
	
	for (auto Element : Target)
	{
		if (DoesItemContainKeyword(Element, KeyWord))
		{
			Result.Add(Element);
		}
	}
	
	return Result;
}

TArray<UDreamInventoryItem*> UDreamGameplayInventoryBlueprintLibrary::FilterItemByType(const TArray<UDreamInventoryItem*>& Target, UDreamInventoryItemType* Type)
{
	TArray<UDreamInventoryItem*> Result;
	
	for (auto Item : Target)
	{
		if (Item->GetType() == Type)
		{
			Result.Add(Item);
		}
	}

	return Result;
}

TArray<UDreamInventoryItem*> UDreamGameplayInventoryBlueprintLibrary::FilterItemByClass(const TArray<UDreamInventoryItem*>& Target, TSubclassOf<UDreamInventoryItem> Class, bool bIncludeSubClass)
{
	TArray<UDreamInventoryItem*> Result;

	if (bIncludeSubClass)
	{
		for (auto Element : Target)
		{
			if (Element->GetClass()->IsChildOf(Class) || Element->StaticClass() == Class)
			{
				Result.Add(Element);
			}
		}
	}
	else
	{
		for (auto Element : Target)
		{
			if (Element->GetClass() == Class)
			{
				Result.Add(Element);
			}
		}
	}

	return Result;
}