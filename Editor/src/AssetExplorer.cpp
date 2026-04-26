#include "AssetExplorer.h"
#include "AssetExplorerDirectory.h"
#include "AssetExplorerFile.h"
#include <kotono_interface/widgets.h>
#include <kotono_io/File.h>
#include <kotono_io/FileExplorer.h>
#include <kotono_input/Mouse.h>

WAssetExplorer::WAssetExplorer()
	: path_("${ENGINE_DIRECTORY}")
	, navigatedPaths_({ path_ })
	, currentPathIndex_(0)
{
}

WidgetPtr WAssetExplorer::Build()
{
	const UFileExplorer fileExplorer(path_);
	const auto directories{ fileExplorer.GetDirectories() };
	const auto files{ fileExplorer.GetFiles() };

	UPtr mainColumn{ UCreate<WColumn>{}() };
	mainColumn->SetSpacing(4.0f);

	UPtr navigationRow{ UCreate<WRow>{}() };
	navigationRow->SetSpacing(4.0f);


	UPtr upBg{ UCreate<WColor>{}() };
	upBg->SetColor(Colors::White.WithAlpha(0.1f));

	UPtr upText{ UCreate<WText>{}() };
	upText->SetText("Up");
	upText->SetFontSize({ 16.0f, 20.0f });

	UPtr upButton{ UCreate<WButton>{}() };
	upButton->SetOnClicked([this]() { Push(path_.Directory()); });


	UPtr previousBg{ UCreate<WColor>{}() };
	previousBg->SetColor(Colors::White.WithAlpha(0.1f));

	UPtr previousText{ UCreate<WText>{}() };
	previousText->SetText("Prev");
	previousText->SetFontSize({ 16.0f, 20.0f });

	UPtr previousButton{ UCreate<WButton>{}() };
	previousButton->SetOnClicked([this]() { NavigatePrevious(); });


	UPtr nextBg{ UCreate<WColor>{}() };
	nextBg->SetColor(Colors::White.WithAlpha(0.1f));

	UPtr nextText{ UCreate<WText>{}() };
	nextText->SetText("Next");
	nextText->SetFontSize({ 16.0f, 20.0f });

	UPtr nextButton{ UCreate<WButton>{}() };
	nextButton->SetOnClicked([this]() { NavigateNext(); });


	UPtr explorerBg{ UCreate<WColor>{}() };
	explorerBg->SetColor(Colors::White.WithAlpha(0.1f));

	UPtr explorerPadding{ UCreate<WPadding>{}() };
	explorerPadding->SetPadding(UPadding::All(8.0f));

	UPtr explorerHorizontalWrapList{ UCreate<WHorizontalWrapList>{}() };
	explorerHorizontalWrapList->SetItemSpacing(10.0f);
	explorerHorizontalWrapList->SetRowSpacing(10.0f);


	std::vector<UWidgetTree*> assets{};
	for (const auto& directory : directories)
	{
		assets.push_back(new UWidgetTreeLeaf(
			UCreate<WAssetExplorerDirectory>{}(directory, [this](const UPath& path) { Push(path); })
		));
	}
	for (const auto& file : files)
	{
		assets.push_back(new UWidgetTreeLeaf(
			UCreate<WAssetExplorerFile>{}(file.Path())
		));
	}

	UChildrenOwnerTree(mainColumn, {
		new UChildrenOwnerTree(navigationRow, {
			new UChildOwnerTree(UCreate<WWrap>{}(),
				new UChildrenOwnerTree(UCreate<WStack>{}(), {
					new UWidgetTreeLeaf(upBg),
					new UWidgetTreeLeaf(upText),
					new UWidgetTreeLeaf(upButton),
				})
			),
			new UChildOwnerTree(UCreate<WWrap>{}(),
				new UChildrenOwnerTree(UCreate<WStack>{}(), {
					new UWidgetTreeLeaf(previousBg),
					new UWidgetTreeLeaf(previousText),
					new UWidgetTreeLeaf(previousButton),
				})
			),
			new UChildOwnerTree(UCreate<WWrap>{}(),
				new UChildrenOwnerTree(UCreate<WStack>{}(), {
					new UWidgetTreeLeaf(nextBg),
					new UWidgetTreeLeaf(nextText),
					new UWidgetTreeLeaf(nextButton),
				})
			),
		}),
		new UChildrenOwnerTree(UCreate<WStack>{}(), {
			new UWidgetTreeLeaf(explorerBg),
			new UChildOwnerTree(explorerPadding,
				new UChildrenOwnerTree(explorerHorizontalWrapList, assets)
			),
		}),
	}).Link();		

	return mainColumn;
}

void WAssetExplorer::Display(UWidgetDisplaySettings displaySettings)
{
	Base::Display(displaySettings);

	Mouse.EventButton(EButton::Previous, EInputState::Pressed).AddListener(this, &WAssetExplorer::OnMousePreviousButtonPressed);
	Mouse.EventButton(EButton::Next, EInputState::Pressed).AddListener(this, &WAssetExplorer::OnMouseNextButtonPressed);
}

void WAssetExplorer::Remove()
{
	Base::Remove();

	Mouse.EventButton(EButton::Previous, EInputState::Pressed).RemoveListener(this, &WAssetExplorer::OnMousePreviousButtonPressed);
	Mouse.EventButton(EButton::Next, EInputState::Pressed).RemoveListener(this, &WAssetExplorer::OnMouseNextButtonPressed);
}

void WAssetExplorer::Push(const UPath& path)
{
	SetState([this, path]()
		{
			path_ = path;
			++currentPathIndex_;
			navigatedPaths_.erase(navigatedPaths_.begin() + currentPathIndex_, navigatedPaths_.end());
			navigatedPaths_.push_back(path);
		});
}

void WAssetExplorer::NavigatePrevious()
{
	if (!navigatedPaths_.empty() && currentPathIndex_ > 0)
	{
		SetState([this]() { path_ = navigatedPaths_[--currentPathIndex_]; });
	}
}

void WAssetExplorer::NavigateNext()
{
	if (!navigatedPaths_.empty() && currentPathIndex_ < navigatedPaths_.size() - 1)
	{
		SetState([this]() { path_ = navigatedPaths_[++currentPathIndex_]; });
	}
}

void WAssetExplorer::OnMousePreviousButtonPressed()
{
	NavigatePrevious();
}

void WAssetExplorer::OnMouseNextButtonPressed()
{
	NavigateNext();
}

#include "generated/AssetExplorer.generated.inl"
