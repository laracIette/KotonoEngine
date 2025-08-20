#pragma once
#include <kotono_framework/glm_includes.h>
class WWidget
{
public:
	struct BuildSettings
	{
		glm::vec2 position;
		glm::vec2 bounds;
		int32_t layer;
	};

	virtual ~WWidget() = default;

	virtual void Build(BuildSettings buildSettings);
	virtual void Destroy() = 0;

	glm::vec2 GetPosition() const;
	glm::vec2 GetSize() const;

protected:
	glm::mat4 GetTranslationMatrix() const;
	glm::mat4 GetRotationMatrix() const;
	glm::mat4 GetScaleMatrix() const;
	glm::mat4 GetModelMatrix() const;

private:
	glm::vec2 position_;
	glm::vec2 size_;
};

