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
	void OnUseCursorForPositionClicked();

	void OnPositionChanged(FVector::FReal NewValue, ETextCommit::Type CommitInfo, int TransformField, EAxisList::Type Axis, bool bCommitted);
	
	void OnXPositionChanged(double);
	TOptional<double> GetXPosition() const;
	void OnYPositionChanged(double);
	TOptional<double> GetYPosition() const;
	void OnZPositionChanged(double);
	TOptional<double> GetZPosition() const;
	void OnResetPosition();


	void OnRotationChanged(FVector::FReal NewValue, ETextCommit::Type CommitInfo, int TransformField, EAxisList::Type Axis, bool bCommitted);
	
	void OnXRotationChanged(double);
	TOptional<double> GetXRotation() const;
	void OnYRotationChanged(double);
	TOptional<double> GetYRotation() const;
	void OnZRotationChanged(double);
	TOptional<double> GetZRotation() const;
	void OnResetRotation();


	void OnScaleChanged(FVector::FReal NewValue, ETextCommit::Type CommitInfo, int TransformField, EAxisList::Type Axis, bool bCommitted);
	
	void OnXScaleChanged(double);
	TOptional<double> GetXScale() const;
	void OnYScaleChanged(double);
	TOptional<double> GetYScale() const;
	void OnZScaleChanged(double);
	TOptional<double> GetZScale() const;
	void OnResetScale();
	
	static FString GetMeshPath();
	static FString GetAIControllerPath();

	FVector GetWorldPositionOfMouse();
	
protected:
	void InitMouseClickEvent();

	FSlateColor GetInnerBackgroundColor() const;
	FSlateColor GetRowBackgroundColor(int32 IndentLevel, bool IsHovered) const;
	
	FTransform TransformToPlace{};

	bool bIsMousePickingLocation{};
	bool bIsTestBoxChecked{};
	FText Name{};
	static FAssetData MeshAssetData;
	static FAssetData AIControllerAssetData;
	TSharedPtr<FAssetThumbnailPool> MeshThumbnailPool;
	TSharedPtr<FAssetThumbnailPool> AIControllerThumbnailPool;
};
