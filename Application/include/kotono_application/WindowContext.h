#pragma once
#include <kotono_common/types.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_platform/Surface.h>
#include <kotono_platform/Window.h>
#include <kotono_rendering/Renderer.h>
#include <string_view>
template <typename T>
class UPtr;
class WWidget;
class WInterfaceRoot;
class UContext;
class UDevice;
class UInterface;
class UWindowContext
{
public:
	UWindowContext(UContext& context, UDevice& device);
	virtual ~UWindowContext();

	void InitInterface(UPtr<WInterfaceRoot> const& widget);
	void Cleanup();

	void Update(f32 deltaTime);
	void DrawFrame();

	// Executes glfwPollEvents() when returns false
	auto GetShouldClose() const -> b8;

	auto GetInterface() const -> UInterface* { return interface_; }

private:
	void OnWindowResized(glm::uvec2 const& extent) const;

protected:
	UWindow window_;
	USurface surface_;
	URenderer renderer_;
	UMouse mouse_;
	UKeyboard keyboard_;

	UInterface* interface_;
};

class UMainWindowContext final : public UWindowContext
{
public:
	using UWindowContext::UWindowContext;

public:
	void InitSurface();
	void InitRenderer();
	void InitInput();
	void InitInterface();

	auto GetSurface() const -> VkSurfaceKHR;
};

class USecondaryWindowContext final : public UWindowContext
{
public:
	using UWindowContext::UWindowContext;

public:
	void Init(glm::uvec2 const& extent, UPtr<WWidget> const& widget);
};
