#include "RPGMakerMenu/SRPGMakerMenu.h"

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

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FAssetData SRPGMakerMenu::MeshAssetData{};
FAssetData SRPGMakerMenu::AIBlackboardAssetData{};
FAssetData SRPGMakerMenu::AnimationBPData{};

void SRPGMakerMenu::Construct(const FArguments& InArgs)
{
	MeshThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
	AIBlackboardThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
	AnimBPThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
	FSlateColor slateBlack = FSlateColor(FLinearColor::FromSRGBColor({0,0,0}));
	
	InitV2Menu();
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
 
void SRPGMakerMenu::OnBlackboardChanged(const FAssetData& _assetData)
{
	AIBlackboardAssetData = _assetData;
}

void SRPGMakerMenu::OnUseCursorForPositionClicked()
{
	bIsMousePickingLocation = true;
}

void SRPGMakerMenu::OnPositionChanged(FVector::FReal NewValue, ETextCommit::Type CommitInfo, int TransformField,
	EAxisList::Type Axis, bool bCommitted)
{
	FVector currentPosition = TransformToPlace.GetLocation();
	auto newVector = FVector((Axis & EAxisList::X) ?  FVector(NewValue).X : currentPosition.X,
	   (Axis & EAxisList::Y) ?  FVector(NewValue).Y : currentPosition.Y,
	   (Axis & EAxisList::Z) ?  FVector(NewValue).Z : currentPosition.Z);
	TransformToPlace.SetTranslation(newVector);
}

void SRPGMakerMenu::OnXPositionChanged(double _newX)
{
	auto translation = TransformToPlace.GetTranslation();
	translation.X = _newX;
	TransformToPlace.SetTranslation(translation);
}

TOptional<double> SRPGMakerMenu::GetXPosition() const
{
	return TransformToPlace.GetTranslation().X;
}

void SRPGMakerMenu::OnYPositionChanged(double _newY)
{
	auto translation = TransformToPlace.GetTranslation();
	translation.Y = _newY;
	TransformToPlace.SetTranslation(translation);
}

TOptional<double> SRPGMakerMenu::GetYPosition() const
{
	return TransformToPlace.GetTranslation().Y;
}

void SRPGMakerMenu::OnZPositionChanged(double _newZ)
{
	auto translation = TransformToPlace.GetTranslation();
	translation.Z = _newZ;
	TransformToPlace.SetTranslation(translation);
}

TOptional<double> SRPGMakerMenu::GetZPosition() const
{
	return TransformToPlace.GetTranslation().Z;
}

void SRPGMakerMenu::OnResetPosition()
{
	TransformToPlace.SetLocation({0.0f, 0.0f, 0.0f});
}

void SRPGMakerMenu::OnRotationChanged(FVector::FReal NewValue, ETextCommit::Type CommitInfo, int TransformField,
	EAxisList::Type Axis, bool bCommitted)
{
	auto currentRotation = TransformToPlace.GetRotation().Rotator();
	auto newVector = FVector((Axis & EAxisList::X) ?  FVector(NewValue).X : currentRotation.Roll,
	   (Axis & EAxisList::Y) ?  FVector(NewValue).Y : currentRotation.Pitch,
	   (Axis & EAxisList::Z) ?  FVector(NewValue).Z : currentRotation.Yaw);
	TransformToPlace.SetRotation(UE::Math::TQuat<double>::MakeFromRotator(FRotator::MakeFromEuler(newVector)));
}

void SRPGMakerMenu::OnResetRotation()
{
	TransformToPlace.SetRotation(UE::Math::TQuat<double>::MakeFromRotator({0.0,0.0,0.0}));
}

void SRPGMakerMenu::OnScaleChanged(FVector::FReal NewValue, ETextCommit::Type CommitInfo, int TransformField,
	EAxisList::Type Axis, bool bCommitted)
{
	FVector currentScale = TransformToPlace.GetScale3D();
	auto newVector = FVector((Axis & EAxisList::X) ?  FVector(NewValue).X : currentScale.X,
	   (Axis & EAxisList::Y) ?  FVector(NewValue).Y : currentScale.Y,
	   (Axis & EAxisList::Z) ?  FVector(NewValue).Z : currentScale.Z);
	TransformToPlace.SetScale3D(newVector);
}

void SRPGMakerMenu::OnResetScale()
{
	TransformToPlace.SetScale3D({1.0f,1.0f,1.0f});
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

void SRPGMakerMenu::OnXScaleChanged(double _newX)
{
	auto scale = TransformToPlace.GetScale3D();
    scale.X = _newX;
    TransformToPlace.SetScale3D(scale);
}

TOptional<double> SRPGMakerMenu::GetXScale() const
{
	return TransformToPlace.GetScale3D().X;
}

void SRPGMakerMenu::OnYScaleChanged(double _newY)
{
	auto scale = TransformToPlace.GetScale3D();
	scale.Y = _newY;
	TransformToPlace.SetScale3D(scale);
}

TOptional<double> SRPGMakerMenu::GetYScale() const
{
	return TransformToPlace.GetScale3D().Y;
}

void SRPGMakerMenu::OnZScaleChanged(double _newZ)
{
	auto scale = TransformToPlace.GetScale3D();
	scale.Z = _newZ;
	TransformToPlace.SetScale3D(scale);
}

TOptional<double> SRPGMakerMenu::GetZScale() const
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

FString SRPGMakerMenu::GetBlackboardPath()
{
	if (AIBlackboardAssetData.IsValid())
	{
		return AIBlackboardAssetData.ObjectPath.ToString();
	}
	return "";
}

FString SRPGMakerMenu::AnimBPPath()
{
	if (AnimationBPData.IsValid())
	{
		return AnimationBPData.ObjectPath.ToString();
	}
	return "";
}

void SRPGMakerMenu::OnAnimBPChanged(const FAssetData& _assetData)
{
	AnimationBPData = _assetData;
}

const SSplitter::FOnSlotResized& SRPGMakerMenu::GetOnNameColumnResized() const
{
	return OnNameColumnResized;
}

FVector SRPGMakerMenu::GetWorldPositionOfMouse()
{
	if (auto client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient())
	{
		auto cursorPos = FSlateApplication::Get().GetCursorPos();
		FVector2D ScreenPosition(cursorPos);
		FVector WorldPosition;
		FVector WorldDirection;
    			
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
			client->Viewport,
			client->GetScene(),
			client->EngineShowFlags));
    
		FSceneView* View = client->CalcSceneView(&ViewFamily);

		View->DeprojectFVector2D(ScreenPosition, /*out*/ WorldPosition, /*out*/ WorldDirection);
		return WorldPosition;
	}

	return {};
}

void SRPGMakerMenu::InitMouseClickEvent()
{
	if (auto client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient())
	{
	}
}

void SRPGMakerMenu::OnSpawnClicked()
{
	// Spawn Character
	UE_LOG(LogTemp, Warning, TEXT("Spawn %s"), *Name.ToString());
	auto* currentLevel = GEditor->GetEditorWorldContext().World()->GetCurrentLevel();
	auto* newNPC = GEditor->AddActor(currentLevel, ACharacter::StaticClass(), TransformToPlace);
	

	// Apply plugin Data
	newNPC->SetActorLabel(Name.ToString());
	if (auto* newCharacter = Cast<ACharacter>(newNPC))
	{
		if (auto* skeletalMeshAsset = Cast<USkeletalMesh>(MeshAssetData.GetAsset()))
		{
			newCharacter->GetMesh()->SetSkeletalMeshAsset(skeletalMeshAsset);
		}

		newCharacter->GetMesh()->SetRelativeLocation({0,0,-90});
		newCharacter->GetMesh()->SetRelativeRotation({0,-90,0});
		newCharacter->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		if (auto* animBP = Cast<UAnimBlueprint>(AnimationBPData.GetAsset()))
		{
			newCharacter->GetMesh()->SetAnimInstanceClass(animBP->GetAnimBlueprintGeneratedClass());
		}
		newCharacter->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
}

void SRPGMakerMenu::OnNameChanged(const FText& _newName)
{
	Name = _newName;
}

FSlateColor SRPGMakerMenu::GetInnerBackgroundColor() const
{
	FSlateColor Color;
	const int32 IndentLevel = 0;
	Color = GetRowBackgroundColor(IndentLevel, this->IsHovered());

	return Color;
}

FSlateColor SRPGMakerMenu::GetRowBackgroundColor(int32 IndentLevel, bool IsHovered) const
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

void SRPGMakerMenu::InitV2Menu()
{
	TSharedRef<SHorizontalBox> NameColumnBox = SNew(SHorizontalBox)
				.Clipping(EWidgetClipping::OnDemand);

	// indentation and expander arrow
	NameColumnBox->AddSlot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Fill)
		.Padding(0)
		.AutoWidth()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Name: "))
		];
	
	TSharedRef<SSplitter> positionSplitter = SNew(SSplitter)
					.Style(FAppStyle::Get(), "DetailsView.Splitter")
					.PhysicalSplitterHandleSize(1.0f)
					.HitDetectionSplitterHandleSize(5.0f);

	positionSplitter->AddSlot()
	.Value(NameColumnData.GetValueColumnWidth())
	.OnSlotResized(NameColumnData.GetOnValueColumnResized())
	[
		NameColumnBox
	];

	
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::Get().GetBrush("DetailsView.CategoryMiddle"))
		.BorderBackgroundColor(GetInnerBackgroundColor())
		.Padding(5)
		[
			positionSplitter
		]
	];
}

void SRPGMakerMenu::InitStandardMenu()
{
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
			.Text(FText::FromString("Animation Blueprint"))
		]
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(SObjectPropertyEntryBox)
			.AllowedClass(UAnimBlueprint::StaticClass())
			.EnableContentPicker(true)
			.DisplayBrowse(true)
			.DisplayThumbnail(true)
			.AllowCreate(true)
			.AllowClear(true)
			.ThumbnailPool(AnimBPThumbnailPool)
			.ObjectPath_Static(&SRPGMakerMenu::AnimBPPath)
			.DisplayUseSelected(true)
			.OnObjectChanged(FOnSetObject::CreateSP(this, &SRPGMakerMenu::OnAnimBPChanged))
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
			.Text(FText::FromString("AI Blackboard"))
		]
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(SObjectPropertyEntryBox)
			.AllowedClass(UBlackboardComponent::StaticClass())
			.EnableContentPicker(true)
			.DisplayBrowse(true)
			.DisplayThumbnail(true)
			.AllowCreate(true)
			.AllowClear(true)
			.ThumbnailPool(AIBlackboardThumbnailPool)
			.ObjectPath_Static(&SRPGMakerMenu::GetBlackboardPath)
			.DisplayUseSelected(true)
			.OnObjectChanged(FOnSetObject::CreateSP(this, &SRPGMakerMenu::OnBlackboardChanged))
		]
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
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
						.Text(FText::FromString("Position (X|Y|Z): "))
					]
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					[
					SNew(SNumericVectorInputBox<FVector::FReal>)
							.X(this, &SRPGMakerMenu::GetXPosition)
							.Y(this, &SRPGMakerMenu::GetYPosition)
							.Z(this, &SRPGMakerMenu::GetZPosition)
							.bColorAxisLabels(true)
							.OnXChanged(this, &SRPGMakerMenu::OnPositionChanged, ETextCommit::Default, 0, EAxisList::X, false)
							.OnYChanged(this, &SRPGMakerMenu::OnPositionChanged, ETextCommit::Default, 0, EAxisList::Y, false)
							.OnZChanged(this, &SRPGMakerMenu::OnPositionChanged, ETextCommit::Default, 0, EAxisList::Z, false)
							.OnXCommitted(this, &SRPGMakerMenu::OnPositionChanged, 0, EAxisList::X, true)
							.OnYCommitted(this, &SRPGMakerMenu::OnPositionChanged, 0, EAxisList::Y, true)
							.OnZCommitted(this, &SRPGMakerMenu::OnPositionChanged, 0, EAxisList::Z, true)
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
						.OnPressed_Raw(this, &SRPGMakerMenu::OnResetPosition)
					]
				]
			]
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
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
						.Text(FText::FromString("Rotation (Y|X|Z): "))
					]
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					[
					SNew(SNumericVectorInputBox<FVector::FReal>)
							.X(this, &SRPGMakerMenu::GetXRotation)
							.Y(this, &SRPGMakerMenu::GetYRotation)
							.Z(this, &SRPGMakerMenu::GetZRotation)
							.bColorAxisLabels(true)
							.OnXChanged(this, &SRPGMakerMenu::OnRotationChanged, ETextCommit::Default, 1, EAxisList::X, false)
							.OnYChanged(this, &SRPGMakerMenu::OnRotationChanged, ETextCommit::Default, 1, EAxisList::Y, false)
							.OnZChanged(this, &SRPGMakerMenu::OnRotationChanged, ETextCommit::Default, 1, EAxisList::Z, false)
							.OnXCommitted(this, &SRPGMakerMenu::OnRotationChanged, 1, EAxisList::X, true)
							.OnYCommitted(this, &SRPGMakerMenu::OnRotationChanged, 1, EAxisList::Y, true)
							.OnZCommitted(this, &SRPGMakerMenu::OnRotationChanged, 1, EAxisList::Z, true)
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
						.OnPressed_Raw(this, &SRPGMakerMenu::OnResetRotation)
					]
				]
			]
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
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
						.Text(FText::FromString("Scale (X|Y|Z): "))
					]
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					[
					SNew(SNumericVectorInputBox<FVector::FReal>)
							.X(this, &SRPGMakerMenu::GetXScale)
							.Y(this, &SRPGMakerMenu::GetYScale)
							.Z(this, &SRPGMakerMenu::GetZScale)
							.bColorAxisLabels(true)
							.OnXChanged(this, &SRPGMakerMenu::OnScaleChanged, ETextCommit::Default, 2, EAxisList::X, false)
							.OnYChanged(this, &SRPGMakerMenu::OnScaleChanged, ETextCommit::Default, 2, EAxisList::Y, false)
							.OnZChanged(this, &SRPGMakerMenu::OnScaleChanged, ETextCommit::Default, 2, EAxisList::Z, false)
							.OnXCommitted(this, &SRPGMakerMenu::OnScaleChanged, 2, EAxisList::X, true)
							.OnYCommitted(this, &SRPGMakerMenu::OnScaleChanged, 2, EAxisList::Y, true)
							.OnZCommitted(this, &SRPGMakerMenu::OnScaleChanged, 2, EAxisList::Z, true)
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
						.OnPressed_Raw(this, &SRPGMakerMenu::OnResetScale)
					]
				]
			]
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
	SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Top)
		[
			SNew(SButton)
			.Text(FText::FromString("Spawn"))
			.OnPressed_Raw(this,  &SRPGMakerMenu::OnSpawnClicked)
		]
	]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
