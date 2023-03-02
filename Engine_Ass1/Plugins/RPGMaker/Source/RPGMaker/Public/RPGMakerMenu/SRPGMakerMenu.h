// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class RPGMAKER_API SRPGMakerMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRPGMakerMenu)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
public: 
	void OnHostileCheckboxChanged(ECheckBoxState NewState);
	void OnMeshChanged(const FAssetData&);
	void OnAIControllerChanged(const FAssetData&);
	static FString GetMeshPath()
	{
		if (MeshAssetData.IsValid())
		{
			return MeshAssetData.ObjectPath.ToString();
		}
		return "";
	}
	static FString GetAIControllerPath()
	{
		if (MeshAssetData.IsValid())
		{
			return MeshAssetData.ObjectPath.ToString();
		}
		return "";
	}
protected:
	bool bIsTestBoxChecked = false;
	static FAssetData MeshAssetData;
	static FAssetData AIControllerAssetData;
	TSharedPtr<FAssetThumbnailPool> MeshThumbnailPool;
	TSharedPtr<FAssetThumbnailPool> AIControllerThumbnailPool;
};
