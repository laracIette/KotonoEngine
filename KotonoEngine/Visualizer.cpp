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

void SVisualizer::Cleanup()
{
}

const bool SVisualizer::GetIsFieldVisible(const EVisualizationField field) const
{
	const auto it = _fieldVisibilities.find(field);
	if (it != _fieldVisibilities.end())
	{
		return it->second;
	}
	return false;
}

void SVisualizer::SetIsFieldVisible(const EVisualizationField field, bool isVisible)
{
	_fieldVisibilities[field] = isVisible;
}
