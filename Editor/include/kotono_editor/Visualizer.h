#pragma once
#include <unordered_map>
#include "VisualizationField.h"
#include <kotono_framework/Event.h>
class SVisualizer final
{
	friend class SEditor;

private:
	void Init();

public:
	bool GetIsFieldVisible(const EVisualizationField field) const; 
	void SetIsFieldVisible(const EVisualizationField field, const bool isVisible);

	KtEvent<bool>& EventVisibilityChanged(const EVisualizationField field);

private:
	std::unordered_map<EVisualizationField, bool> _fieldVisibilities;
	std::unordered_map<EVisualizationField, KtEvent<bool>> eventsVisibilityChanged_;
};

