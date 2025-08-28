#pragma once
#include <functional>
#include "utils.h"
#include <kotono_framework/Cached.h>
#include <kotono_framework/Event.h>
/// Base class of all widgets
class WWidget
{
	friend class VView;

	using StateFunction = std::function<void()>;

public:
	struct BuildSettings
	{
		glm::vec2 position;
		glm::vec2 bounds;
		int32_t layer;
	};

	WWidget();
	virtual ~WWidget() = default;

	virtual WWidget* Build();

	virtual void Display(BuildSettings buildSettings);

	virtual void Cleanup();

	glm::vec2 GetPosition() const;
	glm::vec2 GetSize() const;

protected:
	void SetState(const StateFunction& function);
	void SetBuildSettings(const BuildSettings& buildSettings);

	glm::mat4 GetTranslationMatrix() const;
	glm::mat4 GetRotationMatrix() const;
	glm::mat4 GetScaleMatrix() const;
	glm::mat4 GetModelMatrix() const;

private:
	BuildSettings buildSettings_;
	KtCached<WWidget*> cachedBuild_;

	void Rebuild();
};

