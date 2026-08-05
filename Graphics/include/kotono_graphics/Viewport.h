#pragma once
#include <glm/vec2.hpp>
#include <kotono_common/Event.h>
#include <kotono_common/types.h>
class UViewport final
{
public:
	const glm::uvec2& GetExtent() const;
	const glm::ivec2& GetOffset() const;

	bool GetKeepAspectRatio() const;
	f32 GetAspectRatio() const;

	UEvent<glm::uvec2>& GetEventExtentUpdated();

	void SetExtent(const glm::uvec2& extent);
	void SetOffset(const glm::ivec2& offset);

	void SetKeepAspectRatio(const bool keepAspectRatio);
	void SetAspectRatio(const f32 aspectRatio);

private:
	glm::uvec2 extent_;
	glm::ivec2 offset_;

	bool keepAspectRatio_;
	f32 aspectRatio_;

	UEvent<glm::uvec2> eventExtentUpdated_;
};
