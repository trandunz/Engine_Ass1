// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailColumnSizeData.h"
#include "IPluginWizardDefinition.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/SCompoundWidget.h"
#include "AssetRegistry/AssetData.h"

/**
 * 
 */
class SLATEDESIGNER_API SPluginDesignerMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPluginDesignerMenu)
	{}
	SLATE_END_ARGS()

	//
	void Construct(const FArguments& InArgs);
	void InitV2Menu();
	
protected:
	
	//
	// Example Misc
	//
	private:
	void OnHostileCheckboxChanged(ECheckBoxState NewState);
	bool bIsTestBoxChecked{};
	

	//
	//	Example input field
	//
	private:
	void OnNameChanged(const FText&);
	FName GetNPCDisplayName();

	void OnPluginNameChanged(const FText&);
	void OnPluginPathChanged(const FString&);
	
	FText Name{};
	FText PluginName{};
	FString PluginFolderPath;
	
	//
	// Example Buttons
	//
	private:
	void OnSpawnClicked();
	void OnCreatePluginClicked();
	void OnBrowseButtonClicked();

	TSharedPtr<class IPluginWizardDefinition> PluginWizardDefinition;
	
	//
	// Example Blueprint Pickers
	//
	private:
	void OnMeshChanged(const FAssetData&);
	static FString GetMeshPath();
	void OnAnimBPChanged(const FAssetData&);
	static FString AnimBPPath();
	void OnBlackboardChanged(const FAssetData&);
	static FString GetBlackboardPath();

	static FAssetData MeshAssetData;
	static FAssetData AIBlackboardAssetData;
	static FAssetData AnimationBPData;

	TSharedPtr<FAssetThumbnailPool> MeshThumbnailPool;
	TSharedPtr<FAssetThumbnailPool> AIBlackboardThumbnailPool;
	TSharedPtr<FAssetThumbnailPool> AnimBPThumbnailPool;

	//
	// Example Transform
	//
	private:
	void OnPositionChanged(FVector::FReal NewValue, ETextCommit::Type CommitInfo, int TransformField, EAxisList::Type Axis, bool bCommitted);
	TOptional<double> GetXPosition() const;
	TOptional<double> GetYPosition() const;
	TOptional<double> GetZPosition() const;
	void OnResetPosition();

	void OnRotationChanged(FVector::FReal NewValue, ETextCommit::Type CommitInfo, int TransformField, EAxisList::Type Axis, bool bCommitted);
	TOptional<double> GetXRotation() const;
	TOptional<double> GetYRotation() const;
	TOptional<double> GetZRotation() const;
	void OnResetRotation();
	
	void OnScaleChanged(FVector::FReal NewValue, ETextCommit::Type CommitInfo, int TransformField, EAxisList::Type Axis, bool bCommitted);
	TOptional<double> GetXScale() const;
	TOptional<double> GetYScale() const;
	TOptional<double> GetZScale() const;
	void OnResetScale();

	FTransform TransformToPlace{};
};
