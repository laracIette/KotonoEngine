#pragma once
#include <map>
#include "VisualizationField.h"
class KVisualizer final
{
public:
	void Init();
	void Cleanup();

	void SetIsFieldVisible(const EVisualizationField field, bool isVisible);

private:
	std::map<EVisualizationField, bool> _fieldVisibilities;
};

