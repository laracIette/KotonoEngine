#pragma once
#include <map>
#include "VisualizationField.h"
class KVisualizer final
{
public:
	void Init();
	void Cleanup();

	const bool GetIsFieldVisible(const EVisualizationField field) const; 

	void SetIsFieldVisible(const EVisualizationField field, bool isVisible);

private:
	std::map<EVisualizationField, bool> _fieldVisibilities;
};

