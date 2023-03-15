#include "RPGMakerMenu/SRPGMakerMenu.h"

#include "AIController.h"
#include "AssetViewWidgets.h"
#include "SAssetView.h"
#include "SlateOptMacros.h"
#include "PropertyCustomizationHelpers.h"
#include "GameFramework/Character.h"
#include "Misc/Attribute.h"
#include "InputCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "Layout/Margin.h"
#include "Fonts/SlateFontInfo.h"
#include "Layout/Visibility.h"
#include "Widgets/SNullWidget.h"
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
			.OnTextChanged(FOnTextChanged::CreateSP(this, &SRPGMakerMenu::OnNameChanged))
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
				.Text(FText::FromString("Position (X|Y|Z): "))
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			[
				SNew(SNumericEntryBox<float>)
				.AllowSpin(true)
				.MinValue(-9999)
				.MaxValue(9999)
				.MinSliderValue(-9999.0f)
				.MaxSliderValue(9999.0f)
				.Value(this, &SRPGMakerMenu::GetXPosition)
				.OnValueChanged(this, &SRPGMakerMenu::OnXPositionChanged)
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			[
				SNew(SNumericEntryBox<float>)
				.AllowSpin(true)
				.MinValue(-9999)
				.MaxValue(9999)
				.MinSliderValue(-9999.0f)
				.MaxSliderValue(9999.0f)
				.Value(this, &SRPGMakerMenu::GetYPosition)
				.OnValueChanged(this, &SRPGMakerMenu::OnYPositionChanged)
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			[
				SNew(SNumericEntryBox<float>)
				.AllowSpin(true)
				.MinValue(-9999)
				.MaxValue(9999)
				.MinSliderValue(-9999.0f)
				.MaxSliderValue(9999.0f)
				.Value(this, &SRPGMakerMenu::GetZPosition)
				.OnValueChanged(this, &SRPGMakerMenu::OnZPositionChanged)
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
				.Text(FText::FromString("Rotation (Y|X|Z): "))
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			[
				SNew(SNumericEntryBox<double>)
				.AllowSpin(true)
				.MinValue(-9999)
				.MaxValue(9999)
				.MinSliderValue(-9999.0f)
				.MaxSliderValue(9999.0f)
				.Value(this, &SRPGMakerMenu::GetXRotation)
				.OnValueChanged(this, &SRPGMakerMenu::OnXRotationChanged)
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			[
				SNew(SNumericEntryBox<double>)
				.AllowSpin(true)
				.MinValue(-9999)
				.MaxValue(9999)
				.MinSliderValue(-9999.0f)
				.MaxSliderValue(9999.0f)
				.Value(this, &SRPGMakerMenu::GetYRotation)
				.OnValueChanged(this, &SRPGMakerMenu::OnYRotationChanged)
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			[
				SNew(SNumericEntryBox<double>)
				.AllowSpin(true)
				.MinValue(-9999)
				.MaxValue(9999)
				.MinSliderValue(-9999.0f)
				.MaxSliderValue(9999.0f)
				.Value(this, &SRPGMakerMenu::GetZRotation)
				.OnValueChanged(this, &SRPGMakerMenu::OnZRotationChanged)
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
				.Text(FText::FromString("Scale (X|Y|Z): "))
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			[
				SNew(SNumericEntryBox<float>)
				.AllowSpin(true)
				.MinValue(-9999)
				.MaxValue(9999)
				.MinSliderValue(-9999.0f)
				.MaxSliderValue(9999.0f)
				.Value(this, &SRPGMakerMenu::GetXScale)
				.OnValueChanged(this, &SRPGMakerMenu::OnXScaleChanged)
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			[
				SNew(SNumericEntryBox<float>)
				.AllowSpin(true)
				.MinValue(-9999)
				.MaxValue(9999)
				.MinSliderValue(-9999.0f)
				.MaxSliderValue(9999.0f)
				.Value(this, &SRPGMakerMenu::GetYScale)
				.OnValueChanged(this, &SRPGMakerMenu::OnYScaleChanged)
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			[
				SNew(SNumericEntryBox<float>)
				.AllowSpin(true)
				.MinValue(-9999)
				.MaxValue(9999)
				.MinSliderValue(-9999.0f)
				.MaxSliderValue(9999.0f)
				.Value(this, &SRPGMakerMenu::GetZScale)
				.OnValueChanged(this, &SRPGMakerMenu::OnZScaleChanged)
			]
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
	SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(SButton)
			.Text(FText::FromString("Spawn"))
			.OnPressed_Raw(this,  &SRPGMakerMenu::OnSpawnClicked)
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

void SRPGMakerMenu::OnXPositionChanged(float _newX)
{
	auto translation = TransformToPlace.GetTranslation();
	translation.X = _newX;
	TransformToPlace.SetTranslation(translation);
}

TOptional<float> SRPGMakerMenu::GetXPosition() const
{
	return TransformToPlace.GetTranslation().X;
}

void SRPGMakerMenu::OnYPositionChanged(float _newY)
{
	auto translation = TransformToPlace.GetTranslation();
	translation.Y = _newY;
	TransformToPlace.SetTranslation(translation);
}

TOptional<float> SRPGMakerMenu::GetYPosition() const
{
	return TransformToPlace.GetTranslation().Y;
}

void SRPGMakerMenu::OnZPositionChanged(float _newZ)
{
	auto translation = TransformToPlace.GetTranslation();
	translation.Z = _newZ;
	TransformToPlace.SetTranslation(translation);
}

TOptional<float> SRPGMakerMenu::GetZPosition() const
{
	return TransformToPlace.GetTranslation().Z;
}

void SRPGMakerMenu::OnResetPosition()
{
	TransformToPlace.SetTranslation({});
}

void SRPGMakerMenu::OnResetRotation()
{
	TransformToPlace.SetRotation({});
}

void SRPGMakerMenu::OnResetScale()
{
	TransformToPlace.SetScale3D({1,1,1});
}

void SRPGMakerMenu::OnXRotationChanged(double _newX)
{
	auto rotation = TransformToPlace.GetRotation();
	auto rotator = rotation.Rotator();
	rotator.Roll = _newX;
	auto newRotation = UE::Math::TQuat<double>::MakeFromRotator(rotator);
	TransformToPlace.SetRotation(newRotation);
}

TOptional<double> SRPGMakerMenu::GetXRotation() const
{
	return TransformToPlace.GetRotation().Euler().X;
}

void SRPGMakerMenu::OnYRotationChanged(double _newY)
{
	auto rotation = TransformToPlace.GetRotation();
	auto rotator = rotation.Rotator();
	rotator.Pitch = _newY;
	auto newRotation = UE::Math::TQuat<double>::MakeFromRotator(rotator);
	TransformToPlace.SetRotation(newRotation);
}

TOptional<double> SRPGMakerMenu::GetYRotation() const
{
	// Convert back to degrees
	return TransformToPlace.GetRotation().Euler().Y;
}

void SRPGMakerMenu::OnZRotationChanged(double _newZ)
{
	auto rotation = TransformToPlace.GetRotation();
	auto rotator = rotation.Rotator();
	rotator.Yaw = _newZ;
	auto newRotation = UE::Math::TQuat<double>::MakeFromRotator(rotator);
	TransformToPlace.SetRotation(newRotation);
}

TOptional<double> SRPGMakerMenu::GetZRotation() const
{
	return TransformToPlace.GetRotation().Euler().Z;
}

void SRPGMakerMenu::OnXScaleChanged(float _newX)
{
	auto scale = TransformToPlace.GetScale3D();
    scale.X = _newX;
    TransformToPlace.SetScale3D(scale);
}

TOptional<float> SRPGMakerMenu::GetXScale() const
{
	return TransformToPlace.GetScale3D().X;
}

void SRPGMakerMenu::OnYScaleChanged(float _newY)
{
	auto scale = TransformToPlace.GetScale3D();
	scale.Y = _newY;
	TransformToPlace.SetScale3D(scale);
}

TOptional<float> SRPGMakerMenu::GetYScale() const
{
	return TransformToPlace.GetScale3D().Y;
}

void SRPGMakerMenu::OnZScaleChanged(float _newZ)
{
	auto scale = TransformToPlace.GetScale3D();
	scale.Z = _newZ;
	TransformToPlace.SetScale3D(scale);
}

TOptional<float> SRPGMakerMenu::GetZScale() const
{
	return TransformToPlace.GetScale3D().Z;
}

FString SRPGMakerMenu::GetMeshPath()
{
	if (MeshAssetData.IsValid())
	{
		return MeshAssetData.ObjectPath.ToString();
	}
	return "";
}

FString SRPGMakerMenu::GetAIControllerPath()
{
	if (MeshAssetData.IsValid())
	{
		return MeshAssetData.ObjectPath.ToString();
	}
	return "";
}

void SRPGMakerMenu::OnSpawnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn %s"), *Name.ToString());
	auto* currentLevel = GEditor->GetEditorWorldContext().World()->GetCurrentLevel();
	auto* newNPC = GEditor->AddActor(currentLevel, ACharacter::StaticClass(), TransformToPlace);
	newNPC->SetActorLabel(Name.ToString());
	if (auto* newCharacter = Cast<ACharacter>(newNPC))
	{
		if (auto* skeletalMeshAsset = Cast<USkeletalMesh>(MeshAssetData.GetAsset()))
		{
			newCharacter->GetMesh()->SetSkeletalMeshAsset(skeletalMeshAsset);
		}

		newCharacter->GetMesh()->SetRelativeLocation({0,0,-90});
		newCharacter->GetMesh()->SetRelativeRotation({0,-90,0});
	}
}

void SRPGMakerMenu::OnNameChanged(const FText& _newName)
{
	Name = _newName;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
