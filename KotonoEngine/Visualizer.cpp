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

void KVisualizer::SetIsFieldVisible(const EVisualizationField field, bool isVisible)
{
	_fieldVisibilities[field] = isVisible;
}
