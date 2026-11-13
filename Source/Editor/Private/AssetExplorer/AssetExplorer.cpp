#include "AssetExplorer.h"

#include "AssetExplorerDirectory/AssetExplorerDirectory.h"
#include "AssetExplorerFile/AssetExplorerFile.h"
#include "core_widgets.h"
#include "File/File.h"
#include "FileExplorer/FileExplorer.h"
#include "SelectableList/SelectableList.h"
#include "SelectableList/SelectableListItem.h"

WAssetExplorer::WAssetExplorer()
	: currentDirectory_{ "${ENGINE_DIRECTORY}" }
	, navigatedPaths_{ currentDirectory_ }
	, currentPathIndex_{ 0 }
{
}

auto WAssetExplorer::Build() -> WidgetPtr
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
						| Apply(&WButton::SetOnClicked, [this](EModifier) { Push(currentDirectory_.Directory()); })
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
						| Apply(&WButton::SetIsEnabled, [this]() { return CanNavigatePrevious(); })
						| Apply(&WButton::SetOnClicked, [this](EModifier) { NavigatePrevious(); })
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
						| Apply(&WButton::SetIsEnabled, [this]() { return CanNavigateNext(); })
						| Apply(&WButton::SetOnClicked, [this](EModifier) { NavigateNext(); })
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
				    itemList_ = UCreate<WSelectableList>{}()
				    | Apply(&WSelectableList::SetContainer, (
				        UCreate<WHorizontalWrapList>{ "Item List" }()
                        | Apply(&WHorizontalWrapList::SetItemSpacing, 10.0f)
                        | Apply(&WHorizontalWrapList::SetRowSpacing, 10.0f)
				    ))
					| (
						MakeItems()
					)
				)
			)
		)
	);
}

auto WAssetExplorer::OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8
{
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
	if (inputState != EInputState::Pressed)
	{
		return INPUT_UNHANDLED;
	}

	return INPUT_HANDLED;
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

auto WAssetExplorer::CanNavigatePrevious() const -> b8
{
	return !navigatedPaths_.empty() && currentPathIndex_ > 0;
}

auto WAssetExplorer::CanNavigateNext() const -> b8
{
	return !navigatedPaths_.empty() && currentPathIndex_ < navigatedPaths_.size() - 1;
}

void WAssetExplorer::NavigatePrevious()
{
	if (CanNavigatePrevious())
	{
		currentDirectory_ = navigatedPaths_[--currentPathIndex_];
		UpdateItemList();
	}
}

void WAssetExplorer::NavigateNext()
{
	if (CanNavigateNext())
	{
		currentDirectory_ = navigatedPaths_[++currentPathIndex_];
		UpdateItemList();
	}
}

auto WAssetExplorer::MakeItems() -> USet<UObjectPtr<WSelectableListItem>>
{
	UFileExplorer const fileExplorer{ currentDirectory_ };
	auto const directories{ fileExplorer.GetDirectories() };
	auto const files{ fileExplorer.GetFiles() };

	std::vector<UObjectPtr<WAssetExplorerItem>> items{};

    for (auto const& directory : directories)
	{
		items.push_back(UCreate<WAssetExplorerDirectory>{}(directory, [this](UPath const& path) { Push(path); }));
	}
	for (auto const& file : files)
	{
		items.push_back(UCreate<WAssetExplorerFile>{}(file.Path()));
	}

	return items
        | std::views::transform([](UObjectPtr<WAssetExplorerItem> const& item) {
            return (
                UCreate<WSelectableListItem>{}()
                | Apply(&WSelectableListItem::SetOnDoubleClicked, [item]() { item->Open(); })
                | (
                    item
                )
            );
        })
        | std::ranges::to<USet>();
}

void WAssetExplorer::UpdateItemList()
{
	if (itemList_)
	{
		UAutoDelete const itemListChildren{ itemList_->GetChildren() };

		itemList_->SetChildren(MakeItems());
	}
}

#include "AssetExplorer.generated.inl"
