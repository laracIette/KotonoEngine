#include "Visualizer.h"

void UVisualizer::Init()
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

auto UVisualizer::GetIsFieldVisible(EVisualizationField field) const -> b8
{
	auto const it{ fieldVisibilities_.find(field) };
	return it != fieldVisibilities_.end() ? it->second : false;
}

void UVisualizer::SetIsFieldVisible(EVisualizationField field, b8 isVisible)
{
	fieldVisibilities_[field] = isVisible;
}
