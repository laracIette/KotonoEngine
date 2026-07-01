#pragma once
#include "VisualizationField.h"
#include <kotono_common/Event.h>
#include <unordered_map>
class GVisualizer final
{
	friend class GEditor;

private:
	void Init();

public:
	bool GetIsFieldVisible(const EVisualizationField field) const; 
	void SetIsFieldVisible(const EVisualizationField field, const bool isVisible);

private:
	std::unordered_map<EVisualizationField, bool> fieldVisibilities_;
};

inline GVisualizer Visualizer;
