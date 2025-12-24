#include "Editor.h"
#include "Camera.h"
#include "MainWindow.h"
#include "Visualizer.h"
#include <kotono_platform/WindowViewport.h>
#include <kotono_timing/Timer.h>
#include <kotono_timing/TimerManager.h>

static SVisualizer Visualizer;
static SCamera Camera;

void SEditor::Init()
{
	::Visualizer.Init();
	::Camera.Init();

	auto& updateTimer{ TimerManager.GetTimer("update time text") };
	updateTimer.SetDuration(1.0f / 24.0f);
	updateTimer.SetIsRepeat(true);
	updateTimer.Start();

	widget_ = new WMainWindow();
	widget_->CacheBuild();
	widget_->Display({
		.position = { 0.0f, 0.0f },
		.bounds = static_cast<glm::vec2>(WindowViewport.GetExtent()),
		.layer = 0,
		.scissor = { glm::zero<glm::ivec2>(), WindowViewport.GetExtent() },
	});
}

void SEditor::Update()
{
}

void SEditor::Cleanup()
{
	widget_->Cleanup();
	delete widget_;

	::Camera.Cleanup();
}

SVisualizer& SEditor::Visualizer()
{
	return ::Visualizer;
}

const SVisualizer& SEditor::Visualizer() const
{
	return ::Visualizer;
}

SCamera& SEditor::Camera()
{
	return ::Camera;
}

const SCamera& SEditor::Camera() const
{
	return ::Camera;
}
