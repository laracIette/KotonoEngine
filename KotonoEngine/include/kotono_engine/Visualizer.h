#pragma once
#include <unordered_map>
#include "VisualizationField.h"
class SVisualizer final
{
	friend class SEngine;

private:
	void Init();
	void Cleanup();

public:
	bool GetIsFieldVisible(const EVisualizationField field) const; 

	void SetIsFieldVisible(const EVisualizationField field, const bool isVisible);

private:
	std::unordered_map<EVisualizationField, bool> _fieldVisibilities;
};

