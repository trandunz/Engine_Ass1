#include "SPluginDesignerMenu.h"
#include "Statics.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "PluginUtils.h"
#include "SlateOptMacros.h"
#include "SourceCodeNavigation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "PluginBrowser/Private/PluginBrowserModule.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/MessageDialog.h"
#include "HAL/FileManager.h"
#include "Interfaces/IMainFrameModule.h"
#include "PluginBrowser/Private/SFilePathBlock.h"
#include "SlateFwd.h"
#include "Input/Reply.h"
#include "Brushes/SlateDynamicImageBrush.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "IPluginWizardDefinition.h"
#include "PluginBrowser/Private/NewPluginDescriptorData.h"
#include "Slate/Public/Framework/Application/SlateApplication.h"
#include "ModuleDescriptor.h"
#include "Animation/AnimBlueprint.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FAssetData SPluginDesignerMenu::MeshAssetData{};
FAssetData SPluginDesignerMenu::AIBlackboardAssetData{};
FAssetData SPluginDesignerMenu::AnimationBPData{};

void SPluginDesignerMenu::Construct(const FArguments& InArgs)
{
	MeshThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
	AIBlackboardThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
	AnimBPThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
	FSlateColor slateBlack = FSlateColor(FLinearColor::FromSRGBColor({0,0,0}));
	
	InitV2Menu();
}

void SPluginDesignerMenu::InitV2Menu()
{
	TSharedRef<SVerticalBox> verticalBox = SNew(SVerticalBox)
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		Statics::CreateNewTitle("Plugin Designer")
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		Statics::CreateNamedButton<SPluginDesignerMenu>(this, "Create New Plugin", &SPluginDesignerMenu::OnPluginNameChanged,&SPluginDesignerMenu::OnCreatePluginClicked)
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		Statics::CreateNewTitle("NPC")
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		Statics::CreateNamedInputField<SPluginDesignerMenu>(this, "Name: ", &SPluginDesignerMenu::OnNameChanged)
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		Statics::CreateBlueprintPicker("Mesh", USkeletalMesh::StaticClass(), &SPluginDesignerMenu::GetMeshPath, FOnSetObject::CreateSP(this, &SPluginDesignerMenu::OnMeshChanged), MeshThumbnailPool)
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		Statics::CreateBlueprintPicker("Animation Blueprint", UAnimBlueprint::StaticClass(), &SPluginDesignerMenu::AnimBPPath , FOnSetObject::CreateSP(this, &SPluginDesignerMenu::OnAnimBPChanged), AnimBPThumbnailPool)
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		Statics::CreateVectorInputField<SPluginDesignerMenu>(this, "Position", &SPluginDesignerMenu::GetXPosition, &SPluginDesignerMenu::GetYPosition, &SPluginDesignerMenu::GetZPosition, &SPluginDesignerMenu::OnPositionChanged, &SPluginDesignerMenu::OnResetPosition)
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		Statics::CreateVectorInputField<SPluginDesignerMenu>(this, "Rotation", &SPluginDesignerMenu::GetXRotation, &SPluginDesignerMenu::GetYRotation, &SPluginDesignerMenu::GetZRotation, &SPluginDesignerMenu::OnRotationChanged, &SPluginDesignerMenu::OnResetRotation, 1)
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		Statics::CreateVectorInputField<SPluginDesignerMenu>(this, "Scale", &SPluginDesignerMenu::GetXScale, &SPluginDesignerMenu::GetYScale, &SPluginDesignerMenu::GetZScale, &SPluginDesignerMenu::OnScaleChanged, &SPluginDesignerMenu::OnResetScale, 2)
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		Statics::CreateButton<SPluginDesignerMenu>(this, "Spawn", &SPluginDesignerMenu::OnSpawnClicked)
	];
	
	ChildSlot
	[
		verticalBox
	];
}

void SPluginDesignerMenu::OnHostileCheckboxChanged(ECheckBoxState NewState)
{
	bIsTestBoxChecked = NewState == ECheckBoxState::Checked ? true : false;
}

void SPluginDesignerMenu::OnNameChanged(const FText& _newName)
{
	Name = _newName;
}

FName SPluginDesignerMenu::GetNPCDisplayName()
{
	return TEXT("NPC");
}

void SPluginDesignerMenu::OnPluginNameChanged(const FText&  _newName)
{
	PluginName = _newName;
}

void SPluginDesignerMenu::OnSpawnClicked()
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
			newCharacter->GetMesh()->SetAnimInstanceClass((UClass*)animBP->GetAnimBlueprintGeneratedClass());
		}
		newCharacter->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
}

void SPluginDesignerMenu::OnCreatePluginClicked()
{
	const FName pluginCreatorTabName( TEXT( "PluginCreator" ) );
	FGlobalTabmanager::Get()->TryInvokeTab(pluginCreatorTabName);
	///
	///
	//const bool bHasModules = PluginWizardDefinition->HasModules();
	//
	//FPluginUtils::FNewPluginParamsWithDescriptor CreationParams;
	//CreationParams.TemplateFolders = PluginWizardDefinition->GetFoldersForSelection();
	//CreationParams.Descriptor.bCanContainContent = true;
//
	//if (bHasModules)
	//{
	//	CreationParams.Descriptor.Modules.Add(FModuleDescriptor(*PluginName.ToString(), PluginWizardDefinition->GetPluginModuleDescriptor(), PluginWizardDefinition->GetPluginLoadingPhase()));
	//}
//
	//CreationParams.Descriptor.FriendlyName = PluginName.ToString();
	//CreationParams.Descriptor.Version = 1;
	//CreationParams.Descriptor.VersionName = TEXT("1.0");
	//CreationParams.Descriptor.Category = TEXT("Other");
//
	//PluginWizardDefinition->GetPluginIconPath(/*out*/ CreationParams.PluginIconPath);
	//CreationParams.Descriptor.CreatedBy = "User";
	//CreationParams.Descriptor.CreatedByURL = "";
	//CreationParams.Descriptor.Description = "";
	//CreationParams.Descriptor.bIsBetaVersion = false;
//
	//FText FailReason;
	//FPluginUtils::FLoadPluginParams LoadParams;
	//LoadParams.bEnablePluginInProject = true;
	//LoadParams.bUpdateProjectPluginSearchPath = true;
	//LoadParams.bSelectInContentBrowser = true;
	//LoadParams.OutFailReason = &FailReason;
	//
	//TSharedPtr<IPlugin> NewPlugin = FPluginUtils::CreateAndLoadNewPlugin(PluginName.ToString(), PluginFolderPath, CreationParams, LoadParams);
	//const bool bSucceeded = NewPlugin.IsValid();
//
//
	//PluginWizardDefinition->PluginCreated(PluginName.ToString(), bSucceeded);
//
	//if (bSucceeded)
	//{
	//	// Let the template create additional assets / modify state after creation
//
	//	// Notify that a new plugin has been created
	//	FPluginBrowserModule& PluginBrowserModule = FPluginBrowserModule::Get();
	//	PluginBrowserModule.BroadcastNewPluginCreated();
//
	//	FNotificationInfo Info(FText::Format(FTextFormat::FromString("PluginCreatedSuccessfully '{0}' was created successfully."), PluginName));
	//	Info.bUseThrobber = false;
	//	Info.ExpireDuration = 8.0f;
	//	FSlateNotificationManager::Get().AddNotification(Info)->SetCompletionState(SNotificationItem::CS_Success);
//
	//	if (bHasModules)
	//	{
	//		FSourceCodeNavigation::OpenModuleSolution();
	//	}
	//}
	//else
	//{
	//	const FText Title = FText::FromString("Failed to create plugin!");
	//	FMessageDialog::Open(EAppMsgType::Ok, FailReason, &Title);
	//}

	
}

FReply SPluginDesignerMenu::OnBrowseButtonClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		void* ParentWindowWindowHandle = NULL;

		IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
		const TSharedPtr<SWindow>& MainFrameParentWindow = MainFrameModule.GetParentWindow();
		if (MainFrameParentWindow.IsValid() && MainFrameParentWindow->GetNativeWindow().IsValid())
		{
			ParentWindowWindowHandle = MainFrameParentWindow->GetNativeWindow()->GetOSWindowHandle();
		}

		FString FolderName;
		const FString Title = "NewPluginBrowseTitle Choose a plugin location";
		const bool bFolderSelected = DesktopPlatform->OpenDirectoryDialog(
			FSlateApplication::Get().FindBestParentWindowHandleForDialogs(AsShared()),
			Title,
			LastBrowsePath,
			FolderName
			);

		if (bFolderSelected)
		{
			LastBrowsePath = FolderName;
			OnFolderPathTextChanged(FText::FromString(FolderName));
		}
	}

	return FReply::Handled();
}

FText SPluginDesignerMenu::GetPluginDestinationPath() const
{
	return FText::FromString(PluginFolderPath);
}

FText SPluginDesignerMenu::GetCurrentPluginName() const
{
	return PluginNameText;
}

void SPluginDesignerMenu::OnFolderPathTextChanged(const FText& InText)
{
	PluginFolderPath = InText.ToString();
	FPaths::MakePlatformFilename(PluginFolderPath);
}

void SPluginDesignerMenu::OnPluginNameTextChanged(const FText& InText)
{
	PluginNameText = InText;
}

void SPluginDesignerMenu::OnMeshChanged(const FAssetData& _assetData)
{
	//UE_LOG(LogTemp, Warning, TEXT(" %s "), *_assetData.ObjectPath.ToString() );
	MeshAssetData = _assetData;
}

FString SPluginDesignerMenu::GetMeshPath()
{
	if (MeshAssetData.IsValid())
	{
		return MeshAssetData.ObjectPath.ToString();
	}
	return "";
}

void SPluginDesignerMenu::OnAnimBPChanged(const FAssetData& _assetData)
{
	AnimationBPData = _assetData;
}

FString SPluginDesignerMenu::AnimBPPath()
{
	if (AnimationBPData.IsValid())
	{
		return AnimationBPData.ObjectPath.ToString();
	}
	return "";
}

void SPluginDesignerMenu::OnBlackboardChanged(const FAssetData& _assetData)
{
	AIBlackboardAssetData = _assetData;
}

FString SPluginDesignerMenu::GetBlackboardPath()
{
	if (AIBlackboardAssetData.IsValid())
	{
		return AIBlackboardAssetData.ObjectPath.ToString();
	}
	return "";
}

void SPluginDesignerMenu::OnPositionChanged(FVector::FReal NewValue, ETextCommit::Type CommitInfo, int TransformField,
	EAxisList::Type Axis, bool bCommitted)
{
	FVector currentPosition = TransformToPlace.GetLocation();
	auto newVector = FVector((Axis & EAxisList::X) ?  FVector(NewValue).X : currentPosition.X,
	   (Axis & EAxisList::Y) ?  FVector(NewValue).Y : currentPosition.Y,
	   (Axis & EAxisList::Z) ?  FVector(NewValue).Z : currentPosition.Z);
	TransformToPlace.SetTranslation(newVector);
}

TOptional<double> SPluginDesignerMenu::GetXPosition() const
{
	return TransformToPlace.GetTranslation().X;
}

TOptional<double> SPluginDesignerMenu::GetYPosition() const
{
	return TransformToPlace.GetTranslation().Y;
}

TOptional<double> SPluginDesignerMenu::GetZPosition() const
{
	return TransformToPlace.GetTranslation().Z;
}

void SPluginDesignerMenu::OnResetPosition()
{
	TransformToPlace.SetLocation({0.0f, 0.0f, 0.0f});
}

void SPluginDesignerMenu::OnRotationChanged(FVector::FReal NewValue, ETextCommit::Type CommitInfo, int TransformField,
	EAxisList::Type Axis, bool bCommitted)
{
	auto currentRotation = TransformToPlace.GetRotation().Rotator();
	auto newVector = FVector((Axis & EAxisList::X) ?  FVector(NewValue).X : currentRotation.Roll,
	   (Axis & EAxisList::Y) ?  FVector(NewValue).Y : currentRotation.Pitch,
	   (Axis & EAxisList::Z) ?  FVector(NewValue).Z : currentRotation.Yaw);
	TransformToPlace.SetRotation(UE::Math::TQuat<double>::MakeFromRotator(FRotator::MakeFromEuler(newVector)));
}

TOptional<double> SPluginDesignerMenu::GetXRotation() const
{
	return TransformToPlace.GetRotation().Euler().X;
}

TOptional<double> SPluginDesignerMenu::GetYRotation() const
{
	// Convert back to degrees
	return TransformToPlace.GetRotation().Euler().Y;
}

TOptional<double> SPluginDesignerMenu::GetZRotation() const
{
	return TransformToPlace.GetRotation().Euler().Z;
}

void SPluginDesignerMenu::OnResetRotation()
{
	TransformToPlace.SetRotation(UE::Math::TQuat<double>::MakeFromRotator({0.0,0.0,0.0}));
}

void SPluginDesignerMenu::OnScaleChanged(FVector::FReal NewValue, ETextCommit::Type CommitInfo, int TransformField,
	EAxisList::Type Axis, bool bCommitted)
{
	FVector currentScale = TransformToPlace.GetScale3D();
	auto newVector = FVector((Axis & EAxisList::X) ?  FVector(NewValue).X : currentScale.X,
	   (Axis & EAxisList::Y) ?  FVector(NewValue).Y : currentScale.Y,
	   (Axis & EAxisList::Z) ?  FVector(NewValue).Z : currentScale.Z);
	TransformToPlace.SetScale3D(newVector);
}

TOptional<double> SPluginDesignerMenu::GetXScale() const
{
	return TransformToPlace.GetScale3D().X;
}

TOptional<double> SPluginDesignerMenu::GetYScale() const
{
	return TransformToPlace.GetScale3D().Y;
}

TOptional<double> SPluginDesignerMenu::GetZScale() const
{
	return TransformToPlace.GetScale3D().Z;
}

void SPluginDesignerMenu::OnResetScale()
{
	TransformToPlace.SetScale3D({1.0f,1.0f,1.0f});
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
