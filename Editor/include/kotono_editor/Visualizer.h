#pragma once
#include "VisualizationField.h"
#include <kotono_common/Event.h>
#include <unordered_map>
class SVisualizer final
{
	friend class SEditor;

private:
	void Init();

public:
	bool GetIsFieldVisible(const EVisualizationField field) const; 
	void SetIsFieldVisible(const EVisualizationField field, const bool isVisible);

private:
	std::unordered_map<EVisualizationField, bool> fieldVisibilities_;
};

inline SVisualizer Visualizer;
