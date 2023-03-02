// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGMakerMenu/SRPGMakerMenu.h"

#include "AIController.h"
#include "AssetViewWidgets.h"
#include "SAssetView.h"
#include "SlateOptMacros.h"
#include "PropertyCustomizationHelpers.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FAssetData SRPGMakerMenu::MeshAssetData{};
FAssetData SRPGMakerMenu::AIControllerAssetData{};

void SRPGMakerMenu::Construct(const FArguments& InArgs)
{
	MeshThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
	AIControllerThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
	ChildSlot
	[
	SNew(SVerticalBox)
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(STextBlock)
			.Text(FText::FromString("NPC"))
			.Justification(ETextJustify::Center)
			.Margin(5)
		]
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Mesh"))
		]
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(SObjectPropertyEntryBox)
			.AllowedClass(USkeletalMesh::StaticClass())
			.EnableContentPicker(true)
			.DisplayBrowse(true)
			.DisplayThumbnail(true)
			.AllowCreate(true)
			.AllowClear(true)
			.ThumbnailPool(MeshThumbnailPool)
			.ObjectPath_Static(&SRPGMakerMenu::GetMeshPath)
			.DisplayUseSelected(true)
			.OnObjectChanged(FOnSetObject::CreateSP(this, &SRPGMakerMenu::OnMeshChanged))
		]
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Name: "))
		]
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(SEditableTextBox)
			.Text(FText::FromString(""))
		]
	]
	];
}


void SRPGMakerMenu::OnHostileCheckboxChanged(ECheckBoxState NewState)
{
	bIsTestBoxChecked = NewState == ECheckBoxState::Checked ? true : false;
}

void SRPGMakerMenu::OnMeshChanged(const FAssetData& _assetData)
{
	//UE_LOG(LogTemp, Warning, TEXT(" %s "), *_assetData.ObjectPath.ToString() );
	MeshAssetData = _assetData;
}

void SRPGMakerMenu::OnAIControllerChanged(const FAssetData& _assetData)
{
	AIControllerAssetData = _assetData;
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
