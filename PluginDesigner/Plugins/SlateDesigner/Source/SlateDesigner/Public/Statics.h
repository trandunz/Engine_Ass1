// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimBlueprint.h"
#include "CoreMinimal.h"
#include "PropertyCustomizationHelpers.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowser/Private/SContentBrowser.h"
#include "ContentBrowser/Private/SPathPicker.h"

class SPluginDesignerMenu;
/**
 * 
 */
class SLATEDESIGNER_API Statics
{
public:
	Statics();
	~Statics();

	static TSharedRef<SHorizontalBox> CreateNewTitle(FString _title);
	
	template<typename T>
	static TSharedRef<SHorizontalBox> CreateNamedInputField(T* _this, FString _name, TDelegate<void(const FText&)>::TMethodPtr<T> _onNameChanged);

	template<typename T>
	static TSharedRef<SHorizontalBox> CreateNamedButton(T* _this, FString _name, TDelegate<void(const FText&)>::TMethodPtr<T> _onNameChanged, TDelegate<void()>::TMethodPtr<T> _onPressed);
	
	static TSharedRef<SHorizontalBox> CreateBlueprintPicker(FString _name, UClass* _filter,  TIdentity_T<TDelegate<FString()>::TFuncPtr<>> _getPathFunc, TDelegate<void(const FAssetData&)> _onValueChanged, TSharedPtr<FAssetThumbnailPool>& _thumbnailPool);

	template<typename T>
	static TSharedRef<SHorizontalBox> CreateVectorInputField(T* _this, FString _name,
		TDelegate<TOptional<double>()>::TConstMethodPtr<T> _getXFunc,
		TDelegate<TOptional<double>()>::TConstMethodPtr<T> _getYFunc,
		TDelegate<TOptional<double>()>::TConstMethodPtr<T> _getZFunc,
		TDelegate<void(double)>::TMethodPtr<T, ETextCommit::Type, int, EAxisList::Type, bool> _onChanged,
		TDelegate<void()>::TMethodPtr<T> _onReset,
		int _vectorType = 0);

	template<typename T>
	static TSharedRef<SHorizontalBox> CreateButton(T* _this, FString _text, TDelegate<void()>::TMethodPtr<T> _onPressed);

	//template<typename T>
	//static TSharedPtr<SPathPicker> CreateFolderPicker(T* _this, TDelegate<void(const FString&)>::TMethodPtr<T> _onPathSelected);
	
	static FSlateColor GetInnerBackgroundColor();
	static FSlateColor GetRowBackgroundColor(int32 IndentLevel, bool IsHovered);
	static const SSplitter::FOnSlotResized& GetOnNameColumnResized();

private:
	static SSplitter::FOnSlotResized OnNameColumnResized;
};
