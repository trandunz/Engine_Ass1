#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

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
	void OnSpawnClicked();
	void OnNameChanged(const FText&);
	void OnAIControllerChanged(const FAssetData&);

	void OnXPositionChanged(float);
	TOptional<float> GetXPosition() const;
	void OnYPositionChanged(float);
	TOptional<float> GetYPosition() const;
	void OnZPositionChanged(float);
	TOptional<float> GetZPosition() const;
	void OnResetPosition();
	
	void OnXRotationChanged(double);
	TOptional<double> GetXRotation() const;
	void OnYRotationChanged(double);
	TOptional<double> GetYRotation() const;
	void OnZRotationChanged(double);
	TOptional<double> GetZRotation() const;
	void OnResetRotation();

	void OnXScaleChanged(float);
	TOptional<float> GetXScale() const;
	void OnYScaleChanged(float);
	TOptional<float> GetYScale() const;
	void OnZScaleChanged(float);
	TOptional<float> GetZScale() const;
	void OnResetScale();
	
	static FString GetMeshPath();
	static FString GetAIControllerPath();
protected:
	FTransform TransformToPlace{};
	
	bool bIsTestBoxChecked = false;
	FText Name{};
	static FAssetData MeshAssetData;
	static FAssetData AIControllerAssetData;
	TSharedPtr<FAssetThumbnailPool> MeshThumbnailPool;
	TSharedPtr<FAssetThumbnailPool> AIControllerThumbnailPool;
};
