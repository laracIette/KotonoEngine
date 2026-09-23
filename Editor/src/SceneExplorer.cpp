#include "SceneExplorer.h"

#include "SceneExplorerAddButton.h"
#include "SceneExplorerItem.h"
#include "SceneExplorerRemoveButton.h"
#include <kotono_core/Scene.h>
#include <kotono_interface/widgets.h>

WidgetPtr WSceneExplorer::Build()
{
	return (
		UCreate<WStack>{}()
		| (
			UCreate<WColor>{}()
			| Apply(&WColor::SetColor, Colors::White.WithValue(0.5f).WithAlpha(0.4f))
		)
		| (
			UCreate<WPadding>{}()
			| Apply(&WPadding::SetPadding, UPadding::All(10.0f))
			| (
				UCreate<WColumn>{}()
				| Apply(&WColumn::SetSpacing, 10.0f)
				| (
					UCreate<WRow>{}()
					| (
						UCreate<WBox>{ "Scene Explorer Add Button Box" }()
						| Apply(&WBox::SetSize, glm::vec2{ 80.0f, 40.0f })
						| (
							UCreate<WSceneExplorerAddButton>{}(GetScene())
						)
					)
					| (
						UCreate<WSpacer>{}(EAxis::Horizontal)
					)
					| (
						UCreate<WBox>{ "Scene Explorer Remove Button Box" }()
						| Apply(&WBox::SetSize, glm::vec2{ 80.0f, 40.0f })
						| (
							UCreate<WSceneExplorerRemoveButton>{}(GetScene())
						)
					)
				)
				| (
					UCreate<WText>{ "Scene Explorer Text" }()
					| Apply(&WText::SetText, "Scene Explorer")
				)
				| (
					UCreate<WPadding>{}()
					| Apply(&WPadding::SetPadding, UPadding::All(5.0f))
					| (
						UCreate<WStack>{}()
						| (
							UCreate<WColor>{}()
							| Apply(&WColor::SetColor, Colors::Black.WithAlpha(0.5f))
						)
						| (
							UCreate<WPadding>{}()
							| Apply(&WPadding::SetPadding, UPadding::All(5.0f))
							| (
								itemList_ = UCreate<WList>{ "Scene Explorer Item List" }()
								| MakeItems(GetScene()->GetSceneObjects())
							)
						)
					)
				)
			)
		)
	);
}

void WSceneExplorer::Display(UWidgetDisplaySettings const& displaySettings)
{
	Base::Display(displaySettings);

	GetScene()->GetEventGameStateChanged().AddListener(this, &Self::OnGameStateChanged);
	GetScene()->GetEventSceneObjectsUpdated().AddListener(this, &Self::UpdateItemList);
}

void WSceneExplorer::Remove()
{
	Base::Remove();

	GetScene()->GetEventGameStateChanged().RemoveListener(this, &Self::OnGameStateChanged);
	GetScene()->GetEventSceneObjectsUpdated().RemoveListener(this, &Self::UpdateItemList);
}

void WSceneExplorer::OnGameStateChanged(EGameState gameState) const
{
	if (gameState != EGameState::Paused)
	{
		UpdateItemList(GetScene()->GetSceneObjects());
	}
}

auto WSceneExplorer::MakeItems(std::span<UPtr<TSceneObject> const> sceneObjects) const -> WidgetSet
{
	return sceneObjects
		| std::views::transform([this](UPtr<TSceneObject> const& sceneObject) {
			return UCreate<WSceneExplorerItem>{}(GetScene(), sceneObject);
		})
		| std::ranges::to<USet>();
}

void WSceneExplorer::UpdateItemList(std::span<UPtr<TSceneObject> const> sceneObjects) const
{
	if (itemList_)
	{
		UAutoDelete<WWidget> const itemListChildren{ itemList_->GetChildren() };

		auto const items{ MakeItems(sceneObjects) };
		itemList_->SetChildren(items);
	}
}

#include "generated/SceneExplorer.generated.inl"
