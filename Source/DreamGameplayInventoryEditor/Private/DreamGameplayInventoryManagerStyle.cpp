#include "DreamGameplayInventoryManagerStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FDreamGameplayInventoryManagerStyle::StyleInstance = nullptr;

void FDreamGameplayInventoryManagerStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FDreamGameplayInventoryManagerStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FDreamGameplayInventoryManagerStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("DreamGameplayInventoryManagerStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.f, 20.f);
const FVector2D Icon40x40(40.f, 40.f);

TSharedRef<class FSlateStyleSet> FDreamGameplayInventoryManagerStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("DreamGameplayInventoryManagerStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("DreamGameplayInventory")->GetBaseDir() / TEXT("Resources" / TEXT("Icons")));

	// Add Style...

	Style->Set("DreamGameplayInventory.Icon", new IMAGE_BRUSH(TEXT("TabBrowser"), Icon20x20));
	Style->Set("DreamGameplayInventory.LoadMemory", new IMAGE_BRUSH(TEXT("ForceLoadMemory"), Icon20x20));
	Style->Set("DreamGameplayInventory.Refresh", new IMAGE_BRUSH(TEXT("Refresh"), Icon20x20));
	Style->Set("DreamGameplayInventory.CreateNewItem", new IMAGE_BRUSH(TEXT("CreateNewItem"), Icon20x20));
	Style->Set("DreamGameplayInventory.Open", new IMAGE_BRUSH(TEXT("OpenIcon"), Icon20x20));

	Style->Set("ClassThumbnail.DreamInventoryItem", new IMAGE_BRUSH(TEXT("Item"), Icon40x40));
	Style->Set("ClassThumbnail.DreamInventoryItemType", new IMAGE_BRUSH(TEXT("Type"), Icon40x40));
	Style->Set("ClassThumbnail.DreamInventoryItemLevel", new IMAGE_BRUSH(TEXT("Level"), Icon40x40));

	Style->Set("ClassIcon.DreamInventoryItem", new IMAGE_BRUSH(TEXT("Item"), Icon16x16));
	Style->Set("ClassIcon.DreamInventoryItemType", new IMAGE_BRUSH(TEXT("Type"), Icon16x16));
	Style->Set("ClassIcon.DreamInventoryItemLevel", new IMAGE_BRUSH(TEXT("Level"), Icon16x16));

	Style->Set("DreamGameplayInventoryEditor.DreamGameplayInventoryManager.TabIcon", new IMAGE_BRUSH("TabBrowser", Icon16x16));
	Style->Set("DreamGameplayInventoryEditor.OpenManager", new IMAGE_BRUSH(TEXT("OpenIcon"), Icon40x40));
	return Style;
}

void FDreamGameplayInventoryManagerStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FDreamGameplayInventoryManagerStyle::Get()
{
	return *StyleInstance;
}
