#include "Visualizer.h"

void GVisualizer::Init()
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

bool GVisualizer::GetIsFieldVisible(const EVisualizationField field) const
{
	const auto it{ fieldVisibilities_.find(field) };
	return it != fieldVisibilities_.end() ? it->second : false;
}

void GVisualizer::SetIsFieldVisible(const EVisualizationField field, const bool isVisible)
{
	fieldVisibilities_[field] = isVisible;
}
