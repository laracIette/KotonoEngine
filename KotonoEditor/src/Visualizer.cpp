#include "Visualizer.h"

void SVisualizer::Init()
{
	SetIsFieldVisible(EVisualizationField::SceneObject, true);
	SetIsFieldVisible(EVisualizationField::SceneObjectBounds, true);
	SetIsFieldVisible(EVisualizationField::SceneObjectCollider, true);
	SetIsFieldVisible(EVisualizationField::SceneObjectWireframe, true);
	SetIsFieldVisible(EVisualizationField::InterfaceObject, true);
	SetIsFieldVisible(EVisualizationField::InterfaceObjectBounds, true);
	SetIsFieldVisible(EVisualizationField::InterfaceObjectCollider, true);
	SetIsFieldVisible(EVisualizationField::InterfaceObjectWireframe, true);
}

bool SVisualizer::GetIsFieldVisible(const EVisualizationField field) const
{
	const auto it{ _fieldVisibilities.find(field) };
	if (it != _fieldVisibilities.end())
	{
		return it->second;
	}
	return false; 
}

void SVisualizer::SetIsFieldVisible(const EVisualizationField field, const bool isVisible)
{
	_fieldVisibilities[field] = isVisible;
	eventsVisibilityChanged_[field].Broadcast(isVisible);
}

KtEvent<bool>& SVisualizer::EventVisibilityChanged(const EVisualizationField field)
{
	return eventsVisibilityChanged_[field];
}
