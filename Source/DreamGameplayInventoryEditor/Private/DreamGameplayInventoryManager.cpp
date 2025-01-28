// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.


#include "DreamGameplayInventoryManager.h"

#include "DreamGameplayInventoryEditorSettings.h"
#include "DreamGameplayInventoryEditorTools.h"
#include "DreamGameplayInventoryLog.h"
#include "DreamGameplayInventoryManagerStyle.h"
#include "IContentBrowserSingleton.h"
#include "SlateOptMacros.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Classes/DreamInventoryItem.h"
#include "Classes/DreamInventoryItemType.h"
#include "Components/WidgetSwitcher.h"

#define MAKE_TEXT_FROM_STRING(Fmt, ...) FText::FromString(FString::Printf(TEXT(Fmt), #__VA_ARGS__))

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<SHorizontalBox> FTools::MakeIconAndTextWidget(const FText& Text, const FSlateBrush* IconBrush, int IconSize, float FontSize)
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.AutoWidth()
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.WidthOverride(IconSize)
			.HeightOverride(IconSize)
			[
				SNew(SImage)
				.Image(IconBrush)
			]
		]

		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(2.f, 0.f)
		[
			SNew(STextBlock)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", FontSize))
			.Text(Text)
		];
}

void SDreamGameplayInventoryManager::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)

		// Hander Start
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(FMargin(5.0f))
		[

			SNew(SBorder)
			.Padding(FMargin(5.0f))
			.BorderBackgroundColor(FLinearColor(.3f, .3f, .3f, 1.0f))
			[
				SNew(SHorizontalBox)

				// Title
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock)
					.Text(MAKE_TEXT_FROM_STRING("Dream Inventory Manager"))
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 20.0f))
				]

				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SNew(STextBlock)
					.Text_Lambda([]()
					{
						return FText::FromString(FString::Printf(TEXT("Version : P%s_M%s_D%s"),
							*UDreamGameplayInventoryEditorSettings::Get()->PluginVersion.ToString(),
							*UDreamGameplayInventoryEditorSettings::Get()->ManagerVersion.ToString(),
							*UDreamGameplayInventoryEditorSettings::Get()->DebuggerVersion.ToString()));
					})
				]

				// ButtonMenu
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Fill)
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SButton)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.OnClicked_Lambda([this]()
						{
							ContentSwitcher->SetActiveWidgetIndex(0);
							return FReply::Handled();
						})
						[
							SNew(STextBlock)
							.Text(MAKE_TEXT_FROM_STRING("Manager"))
						]
					]

					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SButton)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.OnClicked_Lambda([this]()
						{
							ContentSwitcher->SetActiveWidgetIndex(1);
							return FReply::Handled();
						})
						[
							SNew(STextBlock)
							.Text(MAKE_TEXT_FROM_STRING("Debugger"))
						]
					]
				]
			]
		] // Hander Ended

		// Content Start
		+ SVerticalBox::Slot()
		.Padding(FMargin(5.0f))
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SBorder)
			.Padding(FMargin(5.0f))
			.BorderBackgroundColor(FLinearColor(.2f, .2f, .2f, 1.0f))
			[
				SAssignNew(ContentSwitcher, SWidgetSwitcher)
				.WidgetIndex(0)

				+ SWidgetSwitcher::Slot()
				[
					SNew(SDreamGameplayInventoryManagerPage)
				]

				+ SWidgetSwitcher::Slot()
				[
					SNew(SDreamGameplayInventoryDebuggerPage)
				]
			]
		] // Content Ended
	];
}

// ------------------------------------------
// Manager
// ------------------------------------------

void SDreamGameplayInventoryManagerPage::Construct(const FArguments& InArgs)
{
	Options.Empty();
	Options.Add(MakeShareable(new FString(TEXT("Item"))));
	Options.Add(MakeShareable(new FString(TEXT("Type"))));
	CurrentSelectedOption = Options[0];

	FDreamGameplayInventoryEditorTools::ForceLoadAssetToMemory(UDreamInventoryItem::StaticClass());
	FDreamGameplayInventoryEditorTools::ForceLoadAssetToMemory(UDreamInventoryItemType::StaticClass());

	RegisterListener();

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBox)
			]

			// TODO : OPTION SELECT
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(ComboBox, SComboBox<FComboBoxType>)
				.OptionsSource(&Options)
				.OnGenerateWidget(this, &SDreamGameplayInventoryManagerPage::MakeOption)
				.OnSelectionChanged(this, &SDreamGameplayInventoryManagerPage::OnSelectionChangedHandle)
				.InitiallySelectedItem(CurrentSelectedOption)
				[
					SNew(STextBlock).Text(this, &SDreamGameplayInventoryManagerPage::GetCurrenItemLabel)
				]
			]

			// TODO : REFRESH
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.OnClicked_Lambda([this]()
				{
					Refresh();
					return FReply::Handled();
				})
				[
					FTools::MakeIconAndTextWidget(MAKE_TEXT_FROM_STRING("Refresh"), FDreamGameplayInventoryManagerStyle::Get().GetBrush("DreamGameplayInventory.Refresh"), 20, 10)
				]
			]

			// TODO : FORCE LOAD MEMORY
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.OnClicked_Lambda([this]()
				{
					FDreamGameplayInventoryEditorTools::ForceLoadAssetToMemory(GetCurrentSelectedClass());
					return FReply::Handled();
				})
				[
					FTools::MakeIconAndTextWidget(MAKE_TEXT_FROM_STRING("ForceLoadToMemory"), FDreamGameplayInventoryManagerStyle::Get().GetBrush("DreamGameplayInventory.LoadMemory"), 20, 10)
				]
			]
		]

		+ SVerticalBox::Slot()
		.VAlign(VAlign_Fill)
		[
			SAssignNew(Switcher, SWidgetSwitcher)

			+ SWidgetSwitcher::Slot()
			[
				SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SErrorText)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 20.0f))
					.ErrorText(MAKE_TEXT_FROM_STRING("Please Refresh."))
				]
			]

			+ SWidgetSwitcher::Slot()
			[
				SAssignNew(ListView, SListView<FRowType>)
				.ListItemsSource(&Data)
				.OnGenerateRow(this, &SDreamGameplayInventoryManagerPage::MakeRow)
				.SelectionMode(ESelectionMode::Type::Single)
				.HeaderRow(
					SAssignNew(HeaderRow, SHeaderRow)
					+ SHeaderRow::Column("Name")
					.DefaultLabel(MAKE_TEXT_FROM_STRING("Name"))
					.SortMode(this, &SDreamGameplayInventoryManagerPage::GetSortMode, FName(TEXT("Name")))
					.OnSort(this, &SDreamGameplayInventoryManagerPage::OnSort)
					.HAlignHeader(HAlign_Center)
					.VAlignHeader(VAlign_Center)
					.HAlignCell(HAlign_Left)
					.VAlignCell(VAlign_Center)
					+ SHeaderRow::Column("Type")
					.DefaultLabel(MAKE_TEXT_FROM_STRING("Type"))
					.SortMode(this, &SDreamGameplayInventoryManagerPage::GetSortMode, FName(TEXT("Type")))
					.OnSort(this, &SDreamGameplayInventoryManagerPage::OnSort)
					.HAlignHeader(HAlign_Center)
					.VAlignHeader(VAlign_Center)
					.HAlignCell(HAlign_Left)
					.VAlignCell(VAlign_Center)
					+ SHeaderRow::Column("Class")
					.DefaultLabel(MAKE_TEXT_FROM_STRING("Class"))
					.SortMode(this, &SDreamGameplayInventoryManagerPage::GetSortMode, FName(TEXT("Class")))
					.OnSort(this, &SDreamGameplayInventoryManagerPage::OnSort)
					.HAlignHeader(HAlign_Center)
					.VAlignHeader(VAlign_Center)
					.HAlignCell(HAlign_Fill)
					.VAlignCell(VAlign_Fill))
			]
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text_Lambda([this]()
				{
					FString ResultText;

					ResultText += FString::Printf(TEXT("Asset Count : %d"), DataCount);

					return FText::FromString(ResultText);
				})
			]
		]
	];

	Refresh();
}

SDreamGameplayInventoryManagerPage::~SDreamGameplayInventoryManagerPage()
{
	UnregisteredListener();
}

void SDreamGameplayInventoryManagerPage::RegisterListener()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	FEditorDelegates::OnAssetsDeleted.AddRaw(this, &SDreamGameplayInventoryManagerPage::OnAssetsDeleteHandle);
	FEditorDelegates::OnNewAssetCreated.AddRaw(this, &SDreamGameplayInventoryManagerPage::OnAssetNewCreate);
	UPackage::PackageSavedWithContextEvent.AddRaw(this, &SDreamGameplayInventoryManagerPage::OnPackageSaved);
}

void SDreamGameplayInventoryManagerPage::UnregisteredListener()
{
	FEditorDelegates::OnAssetsDeleted.RemoveAll(this);
	FEditorDelegates::OnNewAssetCreated.RemoveAll(this);
	UPackage::PackageSavedWithContextEvent.RemoveAll(this);
}

void SDreamGameplayInventoryManagerPage::Refresh()
{
	Data.Empty();
	ListView->RequestListRefresh();

	UClass* SelectedClass = GetCurrentSelectedClass();

	FDreamGameplayInventoryEditorTools::ForceLoadAssetToMemory(SelectedClass);

	TArray<FAssetData> AssetData = FDreamGameplayInventoryEditorTools::GetAssetData(SelectedClass);
	DataCount = AssetData.Num();

	if (SelectedClass == UDreamInventoryItem::StaticClass())
	{
		for (auto Element : AssetData)
		{
			UBlueprint* Blueprint = Cast<UBlueprint>(Element.FastGetAsset(true));

			if (!Blueprint) continue;
			if (!Blueprint->ParentClass) continue;

			if (Blueprint->ParentClass->IsChildOf(SelectedClass) || Blueprint->ParentClass == SelectedClass)
			{
				FRowType Struct = MakeShared<FDreamGameplayItemData>();
				Struct->DataType = EDreamGameplayItemDataType::Item;

				if (UObject* Object = Blueprint->GeneratedClass.GetDefaultObject())
				{
					UDreamInventoryItem* Item = Cast<UDreamInventoryItem>(Object);

					Struct->BP = Blueprint;
					Struct->Item = Item;
					Data.Add(Struct);
				}
			}
		}
	}
	else if (SelectedClass == UDreamInventoryItemType::StaticClass())
	{
		for (auto Element : AssetData)
		{
			if (UDreamInventoryItemType* Type = Cast<UDreamInventoryItemType>(Element.FastGetAsset(true)))
			{
				FRowType Struct = MakeShared<FDreamGameplayItemData>();
				Struct->DataType = EDreamGameplayItemDataType::Type;
				Struct->Type = Type;

				Data.Add(Struct);
			}
		}
	}

	if (Data.Num() > 1)
	{
		Data.Sort([SelectedClass](const FRowType& A, const FRowType& B)
		{
			if (SelectedClass == UDreamInventoryItem::StaticClass())
			{
				return A->Item->Information.Name.ToString() < B->Item->Information.Name.ToString() || A->BP->GetName() < B->BP->GetName();
			}
			else
			{
				return A->Type->TypeName.ToString() < B->Type->TypeName.ToString();
			}
		});
	}

	ListView->RequestListRefresh();
	Switcher->SetActiveWidgetIndex(1);
}

void SDreamGameplayInventoryManagerPage::OnSelectionChangedHandle(FComboBoxType NewValue, ESelectInfo::Type SelectedInfo)
{
	if (NewValue.IsValid())
	{
		CurrentSelectedOption = NewValue;
	}
	Refresh();
}

void SDreamGameplayInventoryManagerPage::OnAssetsDeleteHandle(const TArray<UClass*>& Assets)
{
	Switcher->SetActiveWidgetIndex(0);
}

void SDreamGameplayInventoryManagerPage::OnAssetNewCreate(UFactory* Factory)
{
	Switcher->SetActiveWidgetIndex(0);
}

void SDreamGameplayInventoryManagerPage::OnPackageSaved(const FString& PackageName, UPackage* Package, FObjectPostSaveContext Context)
{
	Switcher->SetActiveWidgetIndex(0);
}

void SDreamGameplayInventoryManagerPage::OnSort(EColumnSortPriority::Type SortPriority, const FName& Column, EColumnSortMode::Type SortType)
{
	FString ColumnString = Column.ToString();

	SortMode = SortType;
	SortTarget = Column;

	if (ColumnString == TEXT("Name")) // Item : Information->Name Type : TypeDisplayName
	{
		Data.Sort([SortType](const FRowType& A, const FRowType& B)
		{
			if (A->DataType == Item && B->DataType == Item)
			{
				if (A->Item && B->Item)
				{
					// Sorting by Name if both items are valid
					if (SortType == EColumnSortMode::Ascending)
					{
						return A->Item->Information.Name.ToString() < B->Item->Information.Name.ToString();
					}
					else
					{
						return A->Item->Information.Name.ToString() > B->Item->Information.Name.ToString();
					}
				}
				else
				{
					return false; // nullptr
				}
			}
			else // DataType == Type
			{
				// Sorting by Type if both types are valid
				if (SortType == EColumnSortMode::Ascending)
				{
					return A->Type->TypeDisplayName.ToString() < B->Type->TypeDisplayName.ToString();
				}
				else
				{
					return A->Type->TypeDisplayName.ToString() > B->Type->TypeDisplayName.ToString();
				}
			}
		});
	}
	else if (ColumnString == TEXT("Type")) // Item : GetType()->TypeName Type : TypeName
	{
		Data.Sort([SortType](const FRowType& A, const FRowType& B)
		{
			// Sorting by Type (TypeName) only if DataType is EDreamGameplayItemDataType::Type
			if (A->DataType == Item && B->DataType == Item)
			{
				if (A->Item->GetType() && B->Item->GetType())
				{
					// Sorting by TypeName
					if (SortType == EColumnSortMode::Ascending)
					{
						return A->Item->GetType()->TypeName.ToString() < B->Item->GetType()->TypeName.ToString();
					}
					else
					{
						return A->Item->GetType()->TypeName.ToString() > B->Item->GetType()->TypeName.ToString();
					}
				}
				else
				{
					return false; // nullptr
				}
			}
			else // DataType == Type
			{
				if (SortType == EColumnSortMode::Ascending)
				{
					return A->Type->TypeName.ToString() < B->Type->TypeName.ToString();
				}
				else
				{
					return A->Type->TypeName.ToString() > B->Type->TypeName.ToString();
				}
			}
		});
	}
	else if (ColumnString == TEXT("Class")) // Item : BP->GetName() Type : Type->GetName()
	{
		Data.Sort([SortType](const FRowType& A, const FRowType& B)
		{
			if (A->DataType == Item && B->DataType == Item)
			{
				if (SortType == EColumnSortMode::Ascending)
				{
					return A->BP->GetName() < B->BP->GetName();
				}
				else
				{
					return A->BP->GetName() > B->BP->GetName();
				}
			}
			else // Data Type == Type
			{
				if (SortType == EColumnSortMode::Ascending)
				{
					return A->Type->GetName() < B->Type->GetName();
				}
				else
				{
					return A->Type->GetName() > B->Type->GetName();
				}
			}
		});
	}

	// Request the list view to refresh after sorting
	ListView->RequestListRefresh();
}

EColumnSortMode::Type SDreamGameplayInventoryManagerPage::GetSortMode(FName ColumnId) const
{
	if (SortTarget != ColumnId)
	{
		return EColumnSortMode::None;
	}

	return SortMode;
}

FText SDreamGameplayInventoryManagerPage::GetCurrenItemLabel() const
{
	if (CurrentSelectedOption.IsValid())
	{
		return FText::FromString(*CurrentSelectedOption);
	}
	else
	{
		return FText::FromString(TEXT("None"));
	}
}

UClass* SDreamGameplayInventoryManagerPage::GetCurrentSelectedClass()
{
	if (CurrentSelectedOption.IsValid())
	{
		if (CurrentSelectedOption == Options[0])
		{
			// Item
			return UDreamInventoryItem::StaticClass();
		}
		else if (CurrentSelectedOption == Options[1])
		{
			// Type
			return UDreamInventoryItemType::StaticClass();
		}
		else
		{
			return nullptr;
		}
	}
	return nullptr;
}

TSharedRef<class SWidget> SDreamGameplayInventoryManagerPage::MakeOption(FComboBoxType InOption)
{
	return SNew(STextBlock).Text(FText::FromString(*InOption));
}

TSharedRef<class ITableRow> SDreamGameplayInventoryManagerPage::MakeRow(TSharedPtr<FDreamGameplayItemData> InRowData, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SDreamGameplayInventoryManagerPageItemRow, OwnerTable).ItemShow(InRowData);
}

// ------------------------------------------
// Row
// ------------------------------------------

void SDreamGameplayInventoryManagerPageItemRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase> InOwnerTableView)
{
	ItemShow = InArgs._ItemShow;

	FSuperRowType::Construct(FSuperRowType::FArguments()
	                         .Padding(0.0f),
	                         InOwnerTableView);
}

TSharedRef<SWidget> SDreamGameplayInventoryManagerPageItemRow::GenerateWidgetForColumn(const FName& ColumnName)
{
	if (ColumnName == "Name")
	{
		return SNew(STextBlock)
			.Text_Lambda([this]()
			{
				switch (ItemShow->DataType)
				{
				case EDreamGameplayItemDataType::Item:
					return FText::FromString(ItemShow->Item->GetInformation().Name.ToString());
					break;
				case EDreamGameplayItemDataType::Type:
					return FText::FromString(ItemShow->Type->TypeDisplayName.ToString());
				default:
					return FText::FromString(TEXT("None"));
				}
			});
	}
	else if (ColumnName == "Type")
	{
		return SNew(STextBlock)
			.Text_Lambda([this]()
			{
				switch (ItemShow->DataType)
				{
				case EDreamGameplayItemDataType::Item:
					return (ItemShow.Get()->Item->GetType() ? FText::FromString(ItemShow->Item->GetType()->TypeName.ToString()) : MAKE_TEXT_FROM_STRING("Null"));
					break;
				case EDreamGameplayItemDataType::Type:
					return FText::FromString(ItemShow.Get()->Type->TypeName.ToString());
					break;
				default:
					return MAKE_TEXT_FROM_STRING("None");
					break;
				}
			});
	}
	else if (ColumnName == "Class")
	{
		FString ClassName = ItemShow.Get()->DataType == EDreamGameplayItemDataType::Item ? ItemShow.Get()->BP->GetName() : ItemShow.Get()->Type->GetName();
		return SNew(SButton)
			.OnClicked_Lambda([this]()
			{
				if (ItemShow->DataType == EDreamGameplayItemDataType::Item)
				{
					GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(ItemShow->BP);
				}
				else
				{
					GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(ItemShow->Type);
				}
				return FReply::Handled();
			})
			.ToolTipText(FText::FromString(FString::Printf(TEXT("Path : %s"), ItemShow->DataType == EDreamGameplayItemDataType::Item ? *ItemShow.Get()->BP->GetPathName() : *ItemShow->Type->GetPathName())))
			[
				FTools::MakeIconAndTextWidget(FText::FromString(ClassName), FDreamGameplayInventoryManagerStyle::Get().GetBrush("DreamGameplayInventory.Open"), 20, 10)
			];
	}

	return SNew(STextBlock)
		.Text(MAKE_TEXT_FROM_STRING("未定义错误"));
}


// ------------------------------------------
// Debugger
// ------------------------------------------

void SDreamGameplayInventoryDebuggerPage::Construct(const FArguments& InArgs)
{
	RegisterLister();

	ChildSlot
	[
		SNew(SHorizontalBox)
		
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(Switcher, SWidgetSwitcher)

			+ SWidgetSwitcher::Slot()
			[
				SNew(SErrorText)
				.ErrorText(MAKE_TEXT_FROM_STRING("Error Please Use In PIE(Play In Editor) Mode."))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 20.0f))
			]

			+ SWidgetSwitcher::Slot()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock)
					.Text(MAKE_TEXT_FROM_STRING("PIE IS RUNNING. (TEST)"))
				]
			]
		]
	];

	if (GWorld->WorldType == EWorldType::Type::PIE)
	{
		Switcher->SetActiveWidgetIndex(1);
	}
}

SDreamGameplayInventoryDebuggerPage::~SDreamGameplayInventoryDebuggerPage()
{
	UnregisterLister();
}

void SDreamGameplayInventoryDebuggerPage::RegisterLister()
{
	FEditorDelegates::PostPIEStarted.AddRaw(this, &SDreamGameplayInventoryDebuggerPage::BeginPIE);
	FEditorDelegates::ShutdownPIE.AddRaw(this, &SDreamGameplayInventoryDebuggerPage::QuitPIE);
}

void SDreamGameplayInventoryDebuggerPage::UnregisterLister()
{
	FEditorDelegates::PostPIEStarted.RemoveAll(this);
	FEditorDelegates::ShutdownPIE.RemoveAll(this);
}

void SDreamGameplayInventoryDebuggerPage::BeginPIE(const bool bIsSimulating)
{
	Switcher->SetActiveWidgetIndex(1);
}

void SDreamGameplayInventoryDebuggerPage::QuitPIE(const bool bIsSimulating)
{
	Switcher->SetActiveWidgetIndex(0);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
