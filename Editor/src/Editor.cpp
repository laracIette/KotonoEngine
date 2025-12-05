#include "Editor.h"
#include "MainWindow.h"
#include <kotono_platform/WindowViewport.h>
#include "GameManager.h"
#include "Visualizer.h"
#include "Camera.h"
#include <kotono_framework/Framework.h>
#include <kotono_timing/TimeManager.h>

static SGameManager GameManager;
static SVisualizer Visualizer;
static SCamera Camera;

void SEditor::Init()
{
	::Visualizer.Init();
	::Camera.Init();

	auto& updateTimer{ Framework.TimeManager().GetTimer("update time text") };
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

SGameManager& SEditor::GameManager()
{
	return ::GameManager;
}

const SGameManager& SEditor::GameManager() const
{
	return ::GameManager;
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
