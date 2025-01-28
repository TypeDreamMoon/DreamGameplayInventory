// Copyright © Dream Moon Studio . Dream Moon All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "DreamInventoryBase.h"
#include "DreamInventoryItem.h"
#include "DreamInventorySlotComponent.generated.h"


/**
 * TODO : 该类未完成
 * TODO : Class Incomplete
 */
UCLASS(ClassGroup=DreamComponent/*, meta=(BlueprintSpawnableComponent)*/)
class DREAMGAMEPLAYINVENTORY_API UDreamInventorySlotComponent : public UDreamInventoryBase
{
	GENERATED_BODY()
public:
	UDreamInventorySlotComponent();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	int ItemSlotCount = 50;

	UFUNCTION(BlueprintCallable, Category = Functions)
	void RefreshSlot();

	UFUNCTION(BlueprintCallable, Category = Functions)
	void InitializeSlot();

	UFUNCTION(BlueprintCallable, Category = Functions)
	bool SwapItemByItem(UDreamInventoryItem* TargetA, UDreamInventoryItem* TargetB);

	UFUNCTION(BlueprintCallable, Category = Functions)
	bool SwapItemByIndex(int TargetIndexA, int TargetIndexB);
public:
	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintCallable, Category = Functions)
	int GiveItemByClass(TSubclassOf<UDreamInventoryItem> InClass, int InCount = 1);

	UFUNCTION(BlueprintCallable, Category = Functions)
	int RemoveItemByClass(TSubclassOf<UDreamInventoryItem> InClass, int InCount = 1);

	UFUNCTION(BlueprintCallable, Category = Functions)
	int RemoveItemByItem(UDreamInventoryItem* InItem, int InCount = 1);

	UFUNCTION(BlueprintCallable, Category = Functions)
	int RemoveItemByIndex(int InIndex, int InCount = 1);
	
	UFUNCTION(BlueprintCallable, Category = Functions)
	int UseItemByClass(TSubclassOf<UDreamInventoryItem> InClass, int InCount = 1);

	UFUNCTION(BlueprintCallable, Category = Functions)
	int UseItemByItem(UDreamInventoryItem* InItem, int InCount = 1);

	UFUNCTION(BlueprintCallable, Category = Functions)
	int UseItemByIndex(int InIndex, int InCount = 1);

	UFUNCTION(BlueprintCallable, Category = Functions)
	void FindEmptySlot(int& FoundIndex, bool& bFound);

	UFUNCTION(BlueprintCallable, Category = Functions)
	bool SplitItem(int SourceIndex, int SplitCount);

	UFUNCTION(BlueprintCallable, Category = Functions)
	void FindsSlotsThatCanBeAdded(TSubclassOf<UDreamInventoryItem> Class, TArray<int>& FoundIndex, bool& bFound);
private:
	static UDreamInventoryItem* MakeEmptySlot();
};
