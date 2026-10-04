#include "WindowContext.h"

#include <kotono_core/Interface.h>

#ifdef EDITOR
#include <kotono_editor/MainWindow.h>
#include <kotono_editor/SecondaryWindow.h>
#endif

UWindowContext::UWindowContext(UContext& context, UDevice& device)
	: window_{}
	, surface_{ window_, context }
	, renderer_{ device, surface_ }
	, mouse_{ window_ }
	, keyboard_{ window_ }
	, interface_{ new UInterface{} }
{
}

UWindowContext::~UWindowContext()
{
	delete interface_;
}

void UWindowContext::InitInterface(UPtr<WInterfaceRoot> const& widget)
{
	window_.GetEventWindowResized().AddListener(this, &UWindowContext::OnWindowResized);

	mouse_.GetEventButton().AddListener(this, &UWindowContext::OnMouseButton);
	keyboard_.GetEventKey().AddListener(this, &UWindowContext::OnKeyboardKey);

	mouse_.GetEventMove().AddListener(interface_, &UInterface::OnMouseMove);
	mouse_.GetEventScroll().AddListener(interface_, &UInterface::OnMouseScroll);

	interface_->SetWidget(widget);
	widget->SetInterface(interface_);

	interface_->BeginDraw(window_.GetExtent());
}

void UWindowContext::Cleanup()
{
	if (interface_)
	{
		mouse_.GetEventButton().RemoveListener(this, &UWindowContext::OnMouseButton);
		keyboard_.GetEventKey().RemoveListener(this, &UWindowContext::OnKeyboardKey);

		mouse_.GetEventMove().RemoveListener(interface_, &UInterface::OnMouseMove);
		mouse_.GetEventScroll().RemoveListener(interface_, &UInterface::OnMouseScroll);

		interface_->EndDraw();
		interface_->Cleanup();
	}

	window_.GetEventWindowResized().RemoveListener(this, &UWindowContext::OnWindowResized);

	keyboard_.Cleanup();
	mouse_.Cleanup();

	renderer_.Cleanup();
	surface_.Cleanup();
	window_.Cleanup();
}

void UWindowContext::Update(f32 deltaTime)
{
	mouse_.Update();
	keyboard_.Update();

	interface_->Update(deltaTime, mouse_.GetCursorPosition());
}

void UWindowContext::DrawFrame()
{
	if (window_.GetIsMinimized())
	{
		return;
	}

	interfaceRenderGraph_.drawDatas.clear();
	interface_->PopulateInterfaceRenderGraph(interfaceRenderGraph_);
	renderer_.DrawFrame(interfaceRenderGraph_);
}

auto UWindowContext::GetShouldClose() const -> b8
{
	return window_.GetShouldClose();
}

void UWindowContext::OnWindowResized(glm::uvec2 const& extent) const
{
	if (interface_)
	{
		interface_->EndDraw();
		interface_->BeginDraw(extent);
	}
}

void UWindowContext::OnMouseButton(EButton button, EInputState inputState) const
{
	if (interface_)
	{
		interface_->OnMouseButton(button, inputState, keyboard_.GetModifier());
	}
}

void UWindowContext::OnKeyboardKey(EKey key, EInputState inputState) const
{
	if (interface_)
	{
		interface_->OnKeyboardKey(key, inputState, keyboard_.GetModifier());
	}
}

void UMainWindowContext::InitSurface()
{
	window_.Init({ 1600u, 900u }, "Kotono Engine");
	surface_.Init();
}

void UMainWindowContext::InitRenderer()
{
	renderer_.Init();
}

void UMainWindowContext::InitInput()
{
	mouse_.Init();
	keyboard_.Init();
}

void UMainWindowContext::InitInterface()
{
	UWindowContext::InitInterface(UCreate<WMainWindow>{ "Main Window" }());
}

auto UMainWindowContext::GetSurface() const -> VkSurfaceKHR
{
	return surface_.GetSurface();
}

void USecondaryWindowContext::Init(glm::uvec2 const& extent, UPtr<WWidget> const& widget, std::string_view name)
{
	window_.Init(extent, std::format("Kotono Engine - {0}", name));
	surface_.Init();
	renderer_.Init();

	mouse_.Init();
	keyboard_.Init();

	InitInterface(UCreate<WSecondaryWindow>{ "Secondary Window" }(widget));
}
