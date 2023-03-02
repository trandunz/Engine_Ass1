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
	void OnObjectChanged(const FAssetData&);
protected:
	bool bIsTestBoxChecked = false;
	FString MeshPath{};
	TSharedPtr<FAssetThumbnailPool> MeshThumbnailPool;
	TSharedPtr<FAssetThumbnailPool> AIControllerThumbnailPool;
};
