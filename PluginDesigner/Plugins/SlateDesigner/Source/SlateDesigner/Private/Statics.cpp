// Fill out your copyright notice in the Description page of Project Settings.


#include "Statics.h"
#include "AIController.h"
#include "AssetViewWidgets.h"
#include "SAssetView.h"
#include "SlateOptMacros.h"
#include "PropertyCustomizationHelpers.h"
#include "GameFramework/Character.h"
#include "Misc/Attribute.h"
#include "InputCoreTypes.h"
#include "LevelEditorViewport.h"
#include "Blueprint/UserWidget.h"
#include "Layout/Margin.h"
#include "Fonts/SlateFontInfo.h"
#include "Layout/Visibility.h"
#include "Widgets/SNullWidget.h"
#include "Editor/PropertyEditor/Private/SDetailRowIndent.h"
#include "Editor/PropertyEditor/Private/SDetailExpanderArrow.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Styling/SlateColor.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Input/NumericTypeInterface.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/SlateTypes.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DetailWidgetRow.h"
#include "Editor.h"
#include "EditorMetadataOverrides.h"
#include "IDetailDragDropHandler.h"
#include "IDetailPropertyExtensionHandler.h"
#include "PropertyEditorModule.h"
#include "Editor/PropertyEditor/Private/SConstrainedBox.h"
#include "Editor/PropertyEditor/Private/SDetailCategoryTableRow.h"

#include "HAL/PlatformApplicationMisc.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Settings/EditorExperimentalSettings.h"
#include "Styling/StyleColors.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "PropertyEditorPermissionList.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Widgets/Input/SVectorInputBox.h"
#include "Math/Transform.h"
#include "UObject/ConstructorHelpers.h"
#include "PropertyEditorModule.h"
#include "Editor/PropertyEditor/Private/UserInterface/PropertyEditor/PropertyEditorConstants.h"


SSplitter::FOnSlotResized Statics::OnNameColumnResized{};

Statics::Statics()
{
}

Statics::~Statics()
{
}

TSharedRef<SBorder> Statics::CreateNewTitle(FString _title)
{
	return SNew(SBorder)
			.Padding(5)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(_title))
					.Margin(10)
				]
			];
}

template<typename T>
TSharedRef<SBorder> Statics::CreateNamedInputField(T* _this, FString _name, TDelegate<void(const FText&)>::TMethodPtr<T> _onNameChanged)
{
	return SNew(SBorder)
			.Padding(5)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Margin(5)
					.Text(FText::FromString(_name))
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				[
					SNew(SEditableTextBox)
					.Text(FText::FromString(""))
					.OnTextChanged(_this, _onNameChanged)
				]
			];
}

template <typename T>
TSharedRef<SBorder> Statics::CreateNamedButton(T* _this, FString _name,
	TDelegate<void(const FText&)>::TMethodPtr<T> _onNameChanged, TDelegate<void()>::TMethodPtr<T> _onPressed)
{
	return SNew(SBorder)
			.Padding(5)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
					.Text(FText::FromString(_name))
					.OnPressed(_this, _onPressed)
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				[
					SNew(SEditableTextBox)
					.Text(FText::FromString(""))
					.OnTextChanged(_this, _onNameChanged)
				]
			];
}

TSharedRef<SBorder> Statics::CreateBlueprintPicker(FString _name, UClass* _filter,
TIdentity_T<TDelegate<FString()>::TFuncPtr<>> _getPathFunc, TDelegate<void(const FAssetData&)> _onValueChanged, TSharedPtr<FAssetThumbnailPool>& _thumbnailPool)
{
	return SNew(SBorder)
			.Padding(5)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Margin(5)
					.Text(FText::FromString(_name))
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				[
					SNew(SObjectPropertyEntryBox)
						.AllowedClass(_filter)
						.EnableContentPicker(true)
						.DisplayBrowse(true)
						.DisplayThumbnail(true)
						.AllowCreate(true)
						.AllowClear(true)
						.ThumbnailPool(_thumbnailPool)
						.ObjectPath_Static(_getPathFunc)
						.DisplayUseSelected(true)
						.OnObjectChanged(_onValueChanged)
				]
			];
}

template <typename T>
TSharedRef<SBorder> Statics::CreateVectorInputField(T* _this, FString _name,
	TDelegate<TOptional<double>()>::TConstMethodPtr<T> _getXFunc,
	TDelegate<TOptional<double>()>::TConstMethodPtr<T> _getYFunc,
	TDelegate<TOptional<double>()>::TConstMethodPtr<T> _getZFunc,
	TDelegate<void(double)>::TMethodPtr<T, ETextCommit::Type, int, EAxisList::Type, bool> _onChanged,
	TDelegate<void()>::TMethodPtr<T> _onReset, int _vectorType)
{
	return SNew(SBorder)
			.Padding(5)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::Get().GetBrush("DetailsView.CategoryMiddle"))
					.BorderBackgroundColor(GetInnerBackgroundColor())
					.Padding(5)
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.AutoWidth()
						.HAlign(HAlign_Left)
						.VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Margin(5)
							.Text(FText::FromString(_name))
						]
						+SHorizontalBox::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Center)
						[
						SNew(SNumericVectorInputBox<FVector::FReal>)
								.X(_this,_getXFunc)
								.Y(_this,_getYFunc)
								.Z(_this,_getZFunc)
								.bColorAxisLabels(true)
								.OnXChanged(_this, _onChanged, ETextCommit::Default, _vectorType, EAxisList::X, false)
								.OnYChanged(_this, _onChanged, ETextCommit::Default, _vectorType, EAxisList::Y, false)
								.OnZChanged(_this, _onChanged, ETextCommit::Default, _vectorType, EAxisList::Z, false)
								.OnXCommitted(_this, _onChanged, _vectorType, EAxisList::X, true)
								.OnYCommitted(_this, _onChanged, _vectorType, EAxisList::Y, true)
								.OnZCommitted(_this, _onChanged, _vectorType, EAxisList::Z, true)
								.AllowSpin(true)
								.SpinDelta(1)
								
						]
						+SHorizontalBox::Slot()
						.AutoWidth()
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Center)
						[
						SNew(SButton)
							.HAlign(HAlign_Center)
							.Text(FText::FromString("Reset"))
							.OnPressed(_this, _onReset)
						]
					]
				]
			];
}

template <typename T>
TSharedRef<SBorder> Statics::CreateButton(T* _this, FString _text, TDelegate<void()>::TMethodPtr<T> _onPressed)
{
	return SNew(SBorder)
			.Padding(5)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
					.Text(FText::FromString(_text))
					.OnPressed(_this, _onPressed)
				]
			];
}

FSlateColor Statics::GetInnerBackgroundColor()
{
	FSlateColor Color;
	const int32 IndentLevel = 0;
	Color = GetRowBackgroundColor(IndentLevel, false);

	return Color;
}

FSlateColor Statics::GetRowBackgroundColor(int32 IndentLevel, bool IsHovered)
{
	int32 ColorIndex = 0;
	int32 Increment = 1;

	for (int i = 0; i < IndentLevel; ++i)
	{
		ColorIndex += Increment;

		if (ColorIndex == 0 || ColorIndex == 3)
		{
			Increment = -Increment;
		}
	}

	static const uint8 ColorOffsets[] =
	{
		0, 4, (4 + 2), (6 + 4), (10 + 6)
	};

	const FSlateColor BaseSlateColor = IsHovered ? 
		FAppStyle::Get().GetSlateColor("Colors.Header") : 
		FAppStyle::Get().GetSlateColor("Colors.Panel");

	const FColor BaseColor = BaseSlateColor.GetSpecifiedColor().ToFColor(true);

	const FColor ColorWithOffset(
		BaseColor.R + ColorOffsets[ColorIndex], 
		BaseColor.G + ColorOffsets[ColorIndex], 
		BaseColor.B + ColorOffsets[ColorIndex]);

	return FSlateColor(FLinearColor::FromSRGBColor(ColorWithOffset));
}

const SSplitter::FOnSlotResized& Statics::GetOnNameColumnResized()
{
	return OnNameColumnResized;
}


