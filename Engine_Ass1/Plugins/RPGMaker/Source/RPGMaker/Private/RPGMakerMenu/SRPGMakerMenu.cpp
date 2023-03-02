// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGMakerMenu/SRPGMakerMenu.h"

#include "AIController.h"
#include "AssetViewWidgets.h"
#include "SAssetView.h"
#include "SlateOptMacros.h"
#include "PropertyCustomizationHelpers.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

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
			.DisplayUseSelected(true)
			.OnObjectChanged(FOnSetObject::CreateSP(this, &SRPGMakerMenu::OnObjectChanged))
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
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Hostile?"))
		]
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(SCheckBox)
			.OnCheckStateChanged(FOnCheckStateChanged::CreateSP(this, &SRPGMakerMenu::OnHostileCheckboxChanged))
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
			.Text(FText::FromString("AI Controller"))
		]
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(SObjectPropertyEntryBox)
			.AllowedClass(AAIController::StaticClass())
			.EnableContentPicker(true)
			.DisplayBrowse(true)
			.DisplayThumbnail(true)
			.AllowCreate(true)
			.AllowClear(true)
			.ThumbnailPool(AIControllerThumbnailPool)
			.DisplayUseSelected(true)
			.OnObjectChanged(FOnSetObject::CreateSP(this, &SRPGMakerMenu::OnObjectChanged))
		]
	]
	];
}


void SRPGMakerMenu::OnHostileCheckboxChanged(ECheckBoxState NewState)
{
	bIsTestBoxChecked = NewState == ECheckBoxState::Checked ? true : false;
}

void SRPGMakerMenu::OnObjectChanged(const FAssetData& _assetData)
{
	MeshPath = _assetData.GetSoftObjectPath().ToString();
	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
