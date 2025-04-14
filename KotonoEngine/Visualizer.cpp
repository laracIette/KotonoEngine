#include "Visualizer.h"

void KVisualizer::Init()
{
	SetIsFieldVisible(EVisualizationField::SceneObject, true);
	SetIsFieldVisible(EVisualizationField::SceneObjectBounds, true);
	SetIsFieldVisible(EVisualizationField::InterfaceObject, true);
	SetIsFieldVisible(EVisualizationField::InterfaceObjectBounds, true);
	SetIsFieldVisible(EVisualizationField::InterfaceCollider, true);
}

void KVisualizer::Cleanup()
{
}

const bool KVisualizer::GetIsFieldVisible(const EVisualizationField field) const
{
	const auto it = _fieldVisibilities.find(field);
	if (it != _fieldVisibilities.end())
	{
		return it->second;
	}
	return false;
}

void KVisualizer::SetIsFieldVisible(const EVisualizationField field, bool isVisible)
{
	_fieldVisibilities[field] = isVisible;
}
