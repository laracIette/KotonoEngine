#include "AssetExplorer.h"

#include "AssetExplorerDirectory.h"
#include "AssetExplorerFile.h"
#include <kotono_interface/widgets.h>
#include <kotono_io/File.h>
#include <kotono_io/FileExplorer.h>

WAssetExplorer::WAssetExplorer()
	: path_{ "${ENGINE_DIRECTORY}" }
	, navigatedPaths_{ path_ }
	, currentPathIndex_{ 0 }
{
}

WidgetPtr WAssetExplorer::Build()
{	
	return UCreate<WColumn>{ "Asset Explorer Main Column" }() 
	| Apply(&WColumn::SetSpacing, 4.0f)
	| (
		UCreate<WRow>{ "Asset Explorer Navigation Row" }()
		| Apply(&WRow::SetSpacing, 4.0f)
		| (
			UCreate<WWrap>{}()
			| (
				UCreate<WStack>{}()
				| (
					UCreate<WColor>{}() 
					| Apply(&WColor::SetColor, Colors::White.WithValue(0.25f))
				)
				| (
					UCreate<WButton>{ "Directory Up Button" }()
					| Apply(&WButton::SetOnClicked, [this]() { Push(path_.Directory()); })
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
					UCreate<WColor>{}()
					| Apply(&WColor::SetColor, Colors::White.WithValue(0.25f))
				)
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
					UCreate<WColor>{}()
					| Apply(&WColor::SetColor, Colors::White.WithValue(0.25f))
				)
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
		| (
			selectOffset_ = UCreate<WOffset>{ "Select Offset" }()
			| (
				selectBox_ = UCreate<WBox>{ "Select Box" }()
				| (
					selectColor_ = UCreate<WColor>{ "Select Box Color" }() 
					| Apply(&WColor::SetColor, UColor::Mix(Colors::Blue, Colors::Cyan).WithAlpha(0.2f))
					| Apply(&WColor::SetIsVisible, false)
				)
			)
		)
	)
	;
}

b8 WAssetExplorer::OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position)
{
	if (Base::OnMouseButton(button, inputState, position))
	{
		return INPUT_HANDLED;
	}

	switch (button)
	{
	case EButton::Left:
	{
		switch (inputState)
		{
		case EInputState::Pressed:
		{
			auto const offset{ position - selectOffset_->GetPosition() };
			selectOffset_->SetOffset(offset);
			selectColor_->SetIsVisible(true);
			return INPUT_HANDLED;
		}
		case EInputState::Released:
		{
			selectColor_->SetIsVisible(false);
			return INPUT_HANDLED;
		}
		case EInputState::Down:
		{
			auto const size{ position - selectBox_->GetPosition() };
			selectBox_->SetSize(size);
			return INPUT_HANDLED;
		}
		default:
			break;
		}
		break;
	}
	case EButton::Previous:
	{
		if (inputState == EInputState::Pressed)
		{
			NavigatePrevious();
			return INPUT_HANDLED;
		}
		break;
	}
	case EButton::Next:
	{
		if (inputState == EInputState::Pressed)
		{
			NavigateNext();
			return INPUT_HANDLED;
		}
		break;
	}
	default:
		break;
	}

	return INPUT_UNHANDLED;
}

void WAssetExplorer::DeselectOthers(UPtr<WAssetExplorerItem> const& item) const
{
	auto assets{ assetExplorerItems_ 
		| std::views::filter([item](auto const& asset) { return asset != item; })
		| std::views::filter(&UPtr<WAssetExplorerItem>::operator bool)
	};

	for (auto const& asset : assets)
	{
		asset->Deselect();
	}
}

void WAssetExplorer::Push(UPath const& path)
{
	path_ = path;
	++currentPathIndex_;
	navigatedPaths_.erase(navigatedPaths_.begin() + currentPathIndex_, navigatedPaths_.end());
	navigatedPaths_.push_back(path);
	UpdateItemList();
}

void WAssetExplorer::NavigatePrevious()
{
	if (!navigatedPaths_.empty() && currentPathIndex_ > 0)
	{
		path_ = navigatedPaths_[--currentPathIndex_];
		UpdateItemList();
	}
}

void WAssetExplorer::NavigateNext()
{
	if (!navigatedPaths_.empty() && currentPathIndex_ < navigatedPaths_.size() - 1)
	{
		path_ = navigatedPaths_[++currentPathIndex_];
		UpdateItemList();
	}
}

auto WAssetExplorer::MakeItems() -> USet<UPtr<WAssetExplorerItem>>
{
	UFileExplorer const fileExplorer{ path_ };
	auto const directories{ fileExplorer.GetDirectories() };
	auto const files{ fileExplorer.GetFiles() };

	USet<UPtr<WAssetExplorerItem>> items{};
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

#include "generated/AssetExplorer.generated.inl"
