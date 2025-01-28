// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UDreamInventoryItemType;
class UDreamInventoryItem;

namespace FTools
{
	static TSharedRef<SHorizontalBox> MakeIconAndTextWidget(const FText& Text, const FSlateBrush* IconBrush, int IconSize, float FontSize);
}

/**
 * 
 */
class DREAMGAMEPLAYINVENTORYEDITOR_API SDreamGameplayInventoryManager : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDreamGameplayInventoryManager)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:
	TSharedPtr<class SWidgetSwitcher> ContentSwitcher;
};

enum EDreamGameplayItemDataType
{
	Item,
	Type,
};
struct FDreamGameplayItemData
{
	EDreamGameplayItemDataType DataType;
	UDreamInventoryItem* Item;
	UDreamInventoryItemType* Type;
	UBlueprint* BP;
};

typedef TSharedPtr<FString> FComboBoxType;
typedef TSharedPtr<FDreamGameplayItemData> FRowType;

class DREAMGAMEPLAYINVENTORYEDITOR_API SDreamGameplayInventoryManagerPage : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDreamGameplayInventoryManagerPage)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	~SDreamGameplayInventoryManagerPage();

protected:
	void RegisterListener();
	void UnregisteredListener();
	void Refresh();
	
	TSharedRef<class ITableRow> MakeRow(FRowType InRowData, const TSharedRef<STableViewBase>& OwnerTable);
	TSharedRef<class SWidget> MakeOption(FComboBoxType InOption);
	
	void OnSelectionChangedHandle(FComboBoxType NewValue, ESelectInfo::Type SelectedInfo);
	void OnAssetsDeleteHandle(const TArray<UClass*>& Assets);
	void OnAssetNewCreate(UFactory* Factory);
	void OnPackageSaved(const FString& PackageName, UPackage* Package, FObjectPostSaveContext Context);
	void OnSort(EColumnSortPriority::Type SortPriority, const FName& Column, EColumnSortMode::Type SortType);
	EColumnSortMode::Type GetSortMode(FName ColumnId) const;
	
	FText GetCurrenItemLabel() const;
	UClass* GetCurrentSelectedClass();
	
	TArray<FComboBoxType> Options;
	TArray<FRowType> Data;
	int DataCount = 0;

	TSharedPtr<SWidgetSwitcher> Switcher;
	TSharedPtr<SListView<FRowType> > ListView;
	TSharedPtr<SComboBox<FComboBoxType>> ComboBox;
	TSharedPtr<SHeaderRow> HeaderRow;

	FComboBoxType CurrentSelectedOption;

	FName SortTarget;
	EColumnSortMode::Type SortMode = EColumnSortMode::Type::Ascending;
};

class DREAMGAMEPLAYINVENTORYEDITOR_API SDreamGameplayInventoryManagerPageItemRow : public SMultiColumnTableRow<TSharedPtr<FDreamGameplayItemData>>
{
	SLATE_BEGIN_ARGS(SDreamGameplayInventoryManagerPageItemRow) : _ItemShow() {}
		SLATE_ARGUMENT(TSharedPtr<FDreamGameplayItemData>, ItemShow)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase> InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;
public:
	TSharedPtr<FDreamGameplayItemData> ItemShow;
};

class DREAMGAMEPLAYINVENTORYEDITOR_API SDreamGameplayInventoryDebuggerPage : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDreamGameplayInventoryDebuggerPage)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	~SDreamGameplayInventoryDebuggerPage();

protected:
	void RegisterLister();
	void UnregisterLister();

	void BeginPIE(const bool bIsSimulating);
	void QuitPIE(const bool bIsSimulating);
	
	TSharedPtr<SWidgetSwitcher> Switcher;
};
