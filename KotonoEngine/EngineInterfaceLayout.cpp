#include "EngineInterfaceLayout.h"
#include "KotonoEngine.h"

void EngineInterfaceLayout::AddComponent(EngineInterfaceComponent* component)
{
	_components.insert(component);
}

void EngineInterfaceLayout::RemoveComponent(EngineInterfaceComponent* component)
{
	_components.erase(component);
}

void EngineInterfaceLayout::OnComponentMoved(EngineInterfaceComponent* movedComponent)
{
	const glm::vec2 cursorPositionNormalized = Engine.GetInputManager().GetMouse().GetCursorPositionNormalized();

	for (auto* component : _components)
	{
		if (component->GetRect().Overlaps(cursorPositionNormalized))
		{
			// docked
			// if outside dead zone
			{
				// set movedComponent position and size accordingly,
				// half component size (width or height)

				const Edge closestEdge = component->GetRect().GetClosestEdge(cursorPositionNormalized);
				
				const glm::vec2 componentSize = component->GetRect().GetSize();
				const glm::vec2 componentPosition = component->GetRect().GetPosition();

				glm::vec2 newSize = componentSize;
				glm::vec2 componentNewPosition = componentPosition;
				glm::vec2 movedComponentNewPosition = componentPosition;

				if (closestEdge == EDGE_LEFT || closestEdge == EDGE_RIGHT)
				{
					newSize.x /= 2.0f;
					const float offset = (closestEdge == EDGE_LEFT) ? newSize.x / 2.0f : -newSize.x / 2.0f;
					componentNewPosition.x += offset;
					movedComponentNewPosition.x -= offset;
				}
				else if (closestEdge == EDGE_TOP || closestEdge == EDGE_BOTTOM)
				{
					newSize.y /= 2.0f;
					const float offset = (closestEdge == EDGE_TOP) ? newSize.y / 2.0f : -newSize.y / 2.0f;
					componentNewPosition.y += offset;
					movedComponentNewPosition.y -= offset;
				}

				component->GetRect().SetSize(newSize);
				component->GetRect().SetPosition(componentNewPosition);
				movedComponent->GetRect().SetSize(newSize);
				movedComponent->GetRect().SetPosition(movedComponentNewPosition);
			}
			// floating
			// else
			{
				// create new window
			}

			break;
		}
	}
}

void EngineInterfaceLayout::Draw() const
{
	for (const auto* component : _components)
	{
		component->Draw();
	}
}
