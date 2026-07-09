#pragma once
#include <glm/fwd.hpp>
#include <kotono_common/types.h>
class GCamera final
{
	friend class GEditor;

private:
	void Init();
	void Cleanup();

private:
	void OnKeyboardWKeyDown() const;
	void OnKeyboardAKeyDown() const;
	void OnKeyboardSKeyDown() const;
	void OnKeyboardDKeyDown() const;
	void OnKeyboardQKeyDown() const;
	void OnKeyboardEKeyDown() const;
	void OnMouseRightButtonPressed();
	void OnMouseRightButtonReleased();
	void OnMouseMove(const glm::vec2 delta);
	void OnMouseVerticalScroll(const f32 delta);

	void Translate(const glm::vec3& delta) const;

private:
	bool isFocused_;

	f32 speed_;
	f32 sensitivity_;
	f32 pitch_;
	f32 yaw_;
};

inline GCamera Camera;
