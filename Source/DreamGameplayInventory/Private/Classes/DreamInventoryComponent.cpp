// Copyright © Dream Moon Studio . Dream Moon All rights reserved


#include "Classes/DreamInventoryComponent.h"

#include "DreamGameplayInventoryLog.h"
#include "DreamGameplayInventoryTools.h"
#include "Classes/DreamInventoryItem.h"

UDreamInventoryComponent::UDreamInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SaveType = EDreamInventorySaveType::EDIST_Default;
}

void UDreamInventoryComponent::GiveItemByClass(TSubclassOf<UDreamInventoryItem> InClass, int Count, bool bReverse)
{
	AddItem(InClass, Count, bReverse);
}

int UDreamInventoryComponent::RemoveItemByClass(TSubclassOf<UDreamInventoryItem> InClass, int Count, bool bReverse)
{
	return RemoveItem(InClass, Count, bReverse);
}

int UDreamInventoryComponent::UseItemByClass(TSubclassOf<UDreamInventoryItem> InClass, int Count, bool bReverse)
{
	return UseItem(InClass, Count, bReverse);
}

int UDreamInventoryComponent::RemoveItemByItem(UDreamInventoryItem* InItem, int Count)
{
	return InItem->RemoveItem(Count);
}

int UDreamInventoryComponent::UseItemByItem(UDreamInventoryItem* InItem, int Count)
{
	return InItem->UseItem(Count);
}

void UDreamInventoryComponent::AddItem(TSubclassOf<UDreamInventoryItem> InClass, int Count, bool bReverse)
{
	if (Count == 0) return;
	
	if (InClass.GetDefaultObject()->GetCanStack())
	{
		// Begin Find
		FDreamInventoryFindResultSpec Spec = UDreamInventoryTools::FindObjectsToAdd(GetInventoryData(), InClass, Count, bReverse);

		if (Spec.IsFound())
		{
			// Found
			if (Spec.GetFindRemaining() > 0)
			{
				GetInventoryData()[Spec.GetFindIndex()]->AddItem(Count - Spec.GetFindRemaining());
				AddItem(InClass, Spec.GetFindRemaining(), bReverse);
			}
			else
			{
				GetInventoryData()[Spec.GetFindIndex()]->AddItem(Count);
			}
		}
		else
		{
			// No Found
			UDreamInventoryItem* Buffer = NewItemByClass(InClass);
			
			int index = GetInventoryData().Add(Buffer);
			
			Buffer->Initialize(this, index);
			
			AddItem(InClass, Count, bReverse);
		}
	}
	else
	{
		for (int i = 0; i < Count; ++i)
		{
			UDreamInventoryItem* Buffer = NewItemByClass(InClass);
			
			int index = GetInventoryData().Add(Buffer);
			
			Buffer->Initialize(this, index);
			
			OnDataChanged.Broadcast(GetInventoryData());
			OnAddItem.Broadcast(Buffer);
		}
	}
}

int UDreamInventoryComponent::RemoveItem(TSubclassOf<UDreamInventoryItem> InClass, int Count, bool bReverse)
{
	if (InClass.GetDefaultObject()->ValueData.bCanStack)
	{
		
		FDreamInventoryFindResultSpec Spec = UDreamInventoryTools::FindObjectsToRemove(GetInventoryData(), InClass, Count, bReverse);

		if (Spec.IsFound())
		{
			if (Spec.GetFindRemaining() > 0)
			{
				GetInventoryData()[Spec.GetFindIndex()]->RemoveItem(Count - Spec.GetFindRemaining());
				return RemoveItem(InClass, Spec.GetFindRemaining(), bReverse);
			}
			else
			{
				GetInventoryData()[Spec.GetFindIndex()]->RemoveItem(Count - Spec.GetFindRemaining());
				return 0;
			}
		}
		else
		{
			return Count;
		}
	}
	else
	{
		int Res = Count;
		for (int i = 0; i < Count; ++i)
		{
			for (auto Element : GetInventoryData())
			{
				if (Element->GetClass() == InClass)
				{
					Element->RemoveItem(1);
					Res--;
					break;
				}
			}
		}
		return Res;
	}
}

int UDreamInventoryComponent::UseItem(TSubclassOf<UDreamInventoryItem> InClass, int Count, bool bReverse)
{
	if (InClass.GetDefaultObject()->ValueData.bCanStack)
	{
		FDreamInventoryFindResultSpec Spec = UDreamInventoryTools::FindObjectsToRemove(GetInventoryData(), InClass, Count, bReverse);

		if (Spec.IsFound())
		{
			if (Spec.GetFindRemaining() > 0)
			{
				GetInventoryData()[Spec.GetFindIndex()]->UseItem(Count - Spec.GetFindRemaining());
				return UseItem(InClass, Spec.FindRemaining, bReverse);
			}
			else
			{
				GetInventoryData()[Spec.GetFindIndex()]->UseItem(Count - Spec.GetFindRemaining());
				return 0;
			}
		}
		else
		{
			return Count;
		}
	}
	else
	{
		int Res = Count;
		for (int i = 0; i < Count; ++i)
		{
			for (auto Element : GetInventoryData())
			{
				if (Element->GetClass() == InClass)
				{
					Element->UseItem(1);
					Res--;
					break;
				}
			}
		}
		return Res;
	}
}
