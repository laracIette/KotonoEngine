#pragma once
#include "VisualizationField.h"
#include <kotono_common/types.h>
#include <unordered_map>
class UVisualizer final
{
public:
	void Init();

	auto GetIsFieldVisible(EVisualizationField field) const -> b8; 
	void SetIsFieldVisible(EVisualizationField field, b8 isVisible);

private:
	std::unordered_map<EVisualizationField, b8> fieldVisibilities_;
};
