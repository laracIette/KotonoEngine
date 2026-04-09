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
	Mouse.EventButton(EButton::Previous, EInputState::Pressed).AddListener(this, &WAssetExplorer::OnMousePreviousButtonPressed);
	Mouse.EventButton(EButton::Next, EInputState::Pressed).AddListener(this, &WAssetExplorer::OnMouseNextButtonPressed);

	const UFileExplorer fileExplorer(path_);
	const auto directories{ fileExplorer.GetDirectories() };
	const auto files{ fileExplorer.GetFiles() };

	return new WColumn({
		.spacing = 4.0f,
		.children = {
			new WRow({
				.spacing = 4.0f,
				.children = {
					new WWrap({
						.child = new WStack({
							.children = {
								new WColor({ Colors::White.WithAlpha(0.1f) }),
								new WText({
									.text = "Up",
									.fontSize = { 16.0f, 20.0f },
								}),
								new WButton({
									.onClicked = [this]() { Push(path_.Directory()); },
								}),
							},
						}),
					}),
					new WWrap({
						.child = new WStack({
							.children = {
								new WColor({ Colors::White.WithAlpha(0.1f) }),
								new WText({
									.text = "Prev",
									.fontSize = { 16.0f, 20.0f },
								}),
								new WButton({
									.onClicked = [this]() { NavigatePrevious(); },
								}),
							},
						}),
					}),
					new WWrap({
						.child = new WStack({
							.children = {
								new WColor({ Colors::White.WithAlpha(0.1f) }),
								new WText({
									.text = "Next",
									.fontSize = { 16.0f, 20.0f },
								}),
								new WButton({
									.onClicked = [this]() { NavigateNext(); },
								}),
							},
						}),
					}),
				},
			}),
			new WStack({
				.children = {
					new WColor({ Colors::White.WithAlpha(0.1f) }),
					new WPadding({
						.padding = WPadding::Padding::All(8.0f),
						.child = new WHorizontalWrapList({
							.itemSpacing = 10.0f,
							.rowSpacing = 10.0f,
							.children = [this, directories, files]() {
								WidgetVector result{};

								for (const auto& directory : directories)
								{
									result.push_back(new WAssetExplorerDirectory(directory, [this](const UPath& path) { Push(path); }));
								}

								for (const auto& file : files)
								{
									result.push_back(new WAssetExplorerFile(file.Path()));
								}

								return result;
							},
						}),
					}),
				},
			}),
		},
	});
		
}

void WAssetExplorer::Cleanup()
{
	Mouse.EventButton(EButton::Previous, EInputState::Pressed).RemoveListener(this, &WAssetExplorer::OnMousePreviousButtonPressed);
	Mouse.EventButton(EButton::Next, EInputState::Pressed).RemoveListener(this, &WAssetExplorer::OnMouseNextButtonPressed);

	Base::Cleanup();
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
