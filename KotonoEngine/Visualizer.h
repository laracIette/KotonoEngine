#pragma once
#include <unordered_map>
#include "VisualizationField.h"
class SVisualizer final
{
public:
	void Init();
	void Cleanup();

	const bool GetIsFieldVisible(const EVisualizationField field) const; 

	void SetIsFieldVisible(const EVisualizationField field, bool isVisible);

private:
	std::unordered_map<EVisualizationField, bool> _fieldVisibilities;
};

