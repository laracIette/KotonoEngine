#include "AssetExplorer.h"

#include "AssetExplorerDirectory/AssetExplorerDirectory.h"
#include "AssetExplorerFile/AssetExplorerFile.h"
#include <core_widgets.h>
#include <File/File.h>
#include <FileExplorer/FileExplorer.h>

WAssetExplorer::WAssetExplorer()
	: currentDirectory_{ "${ENGINE_DIRECTORY}" }
	, navigatedPaths_{ currentDirectory_ }
	, currentPathIndex_{ 0 }
{
}

WidgetPtr WAssetExplorer::Build()
{			
	return (
		UCreate<WColumn>{ "Asset Explorer Main Column" }() 
		| Apply(&WColumn::SetSpacing, 4.0f)
		| (
			UCreate<WRow>{ "Asset Explorer Navigation Row" }()
			| Apply(&WRow::SetSpacing, 4.0f)
			| (
				UCreate<WWrap>{}()
				| (
					UCreate<WStack>{}()
					| (
						UCreate<WButton>{ "Directory Up Button" }()
						| Apply(&WButton::SetOnClicked, [this]() { Push(currentDirectory_.Directory()); })
					)
					| (
						UCreate<WText>{ "Directory Up Text" }()
						| Apply(&WText::SetText, "Up")
						| Apply(&WText::SetFontSize, glm::vec2{ 16.0f, 20.0f })
					)
				)
			)
			| (
				UCreate<WWrap>{}()
				| (
					UCreate<WStack>{}()
					| (
						UCreate<WButton>{ "Directory Prev Button" }()
						| Apply(&WButton::SetOnClicked, [this]() { NavigatePrevious(); })
					)
					| (
						UCreate<WText>{ "Directory Prev Text" }()
						| Apply(&WText::SetText, "Prev")
						| Apply(&WText::SetFontSize, glm::vec2{ 16.0f, 20.0f })
					)
				)
			)
			| (
				UCreate<WWrap>{}()
				| (
					UCreate<WStack>{}()
					| (
						UCreate<WButton>{ "Directory Next Button" }()
						| Apply(&WButton::SetOnClicked, [this]() { NavigateNext(); })
					)
					| (
						UCreate<WText>{ "Directory Next Text" }()
						| Apply(&WText::SetText, "Next")
						| Apply(&WText::SetFontSize, glm::vec2{ 16.0f, 20.0f })
					)
				)
			)
		)
		| (
			UCreate<WStack>{ "Item List Stack" }()
			| (
				UCreate<WColor>{ "Item List Background" }() 
				| Apply(&WColor::SetColor, Colors::White.WithValue(0.05f))
			)
			| (
				UCreate<WPadding>{ "Item List Padding" }()
				| Apply(&WPadding::SetPadding, UPadding::All(8.0f))
				| (
					itemList_ = UCreate<WHorizontalWrapList>{ "Item List" }()
					| Apply(&WHorizontalWrapList::SetItemSpacing, 10.0f)
					| Apply(&WHorizontalWrapList::SetRowSpacing, 10.0f)
					| (assetExplorerItems_ = MakeItems())
				)
			)
		)
	);
}

auto WAssetExplorer::OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8
{
	if (Base::OnMouseButton(button, inputState, modifier))
	{
		return INPUT_HANDLED;
	}

	if (inputState != EInputState::Pressed)
	{
		return INPUT_UNHANDLED;
	}

	switch (button)
	{
	case EButton::Previous:
	{
		NavigatePrevious();
		return INPUT_HANDLED;
	}
	case EButton::Next:
	{
		NavigateNext();
		return INPUT_HANDLED;
	}
	default:
		break;
	}

	return INPUT_UNHANDLED;
}

auto WAssetExplorer::OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) -> b8
{
	if (!GetIsFocused())
	{
		return INPUT_UNHANDLED;
	}
	
	if (inputState != EInputState::Pressed)
	{
		return INPUT_UNHANDLED;
	}

	switch (key)
	{
	case EKey::Enter:
		break;
	default:
		break;
	}

	return INPUT_HANDLED;
}

void WAssetExplorer::DeselectOthers(AssetExplorerItem const& item) const
{
	auto assets{ assetExplorerItems_ 
		| std::views::filter([item](AssetExplorerItem const& asset) { return asset != item; })
		| std::views::filter(&AssetExplorerItem::operator b8)
	};

	for (auto const& asset : assets)
	{
		asset->Deselect();
	}
}

void WAssetExplorer::Push(UPath const& path)
{
	Check(Throw, path.Exists(), "path is invalid");

	if (path == currentDirectory_)
	{
		return;
	}

	currentDirectory_ = path;
	++currentPathIndex_;
	navigatedPaths_.erase(navigatedPaths_.begin() + currentPathIndex_, navigatedPaths_.end());
	navigatedPaths_.push_back(path);
	UpdateItemList();
}

void WAssetExplorer::NavigatePrevious()
{
	if (!navigatedPaths_.empty() && currentPathIndex_ > 0)
	{
		currentDirectory_ = navigatedPaths_[--currentPathIndex_];
		UpdateItemList();
	}
}

void WAssetExplorer::NavigateNext()
{
	if (!navigatedPaths_.empty() && currentPathIndex_ < navigatedPaths_.size() - 1)
	{
		currentDirectory_ = navigatedPaths_[++currentPathIndex_];
		UpdateItemList();
	}
}

auto WAssetExplorer::MakeItems() -> USet<AssetExplorerItem>
{
	UFileExplorer const fileExplorer{ currentDirectory_ };
	auto const directories{ fileExplorer.GetDirectories() };
	auto const files{ fileExplorer.GetFiles() };

	USet<AssetExplorerItem> items{};
	for (auto const& directory : directories)
	{
		items.Add(UCreate<WAssetExplorerDirectory>{}(Ptr(), directory, [this](UPath const& path) { Push(path); }));
	}
	for (auto const& file : files)
	{
		items.Add(UCreate<WAssetExplorerFile>{}(Ptr(), file.Path()));
	}
	return items;
}

void WAssetExplorer::UpdateItemList()
{
	if (itemList_)
	{
		UAutoDelete<WWidget> const itemListChildren{ itemList_->GetChildren() };

		assetExplorerItems_ = MakeItems();
		itemList_->SetChildren(assetExplorerItems_);
	}
}

#include "AssetExplorer.generated.inl"
