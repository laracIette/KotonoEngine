#include "Editor.h"
#include "Camera.h"
#include "MainWindow.h"
#include "Visualizer.h"
#include <kotono_platform/Window.h>
#include <kotono_platform/WindowViewport.h>
#include <kotono_timing/Timer.h>
#include <kotono_timing/TimerManager.h>

void SEditor::Init()
{
	Visualizer.Init();
	Camera.Init();

	auto& updateTimer{ TimerManager.GetTimer("update time text") };
	updateTimer.SetDuration(1.0f / 20.0f);
	updateTimer.SetIsRepeat(true);
	updateTimer.Start();

	CreateWidget();

	Window.GetEventWindowResized().AddListener(KtDelegate(this, &SEditor::RefreshMainWindowWidget));
}

void SEditor::Update()
{
}

void SEditor::Cleanup()
{
	DeleteWidget();

	Camera.Cleanup();

	Window.GetEventWindowResized().RemoveListener(KtDelegate(this, &SEditor::RefreshMainWindowWidget));
}

void SEditor::CreateWidget()
{
	widget_ = new WMainWindow();
	widget_->CacheBuild();
	widget_->Display({
		.position = { 0.0f, 0.0f },
		.bounds = static_cast<glm::vec2>(WindowViewport.GetExtent()),
		.layer = 0,
		.scissor = { glm::zero<glm::ivec2>(), WindowViewport.GetExtent() },
	});
}

void SEditor::DeleteWidget()
{
	if (!widget_)
	{
		return;
	}
	widget_->Cleanup();
	delete widget_;
}

void SEditor::RefreshMainWindowWidget()
{
	DeleteWidget();
	CreateWidget();
}
