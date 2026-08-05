#include "RenderContext.h"
#include <kotono_timing/Clock.h>

void URenderContext::Init()
{
    renderer_.Init();
}

void URenderContext::Cleanup()
{
    renderer_.Cleanup();
}

URenderer& URenderContext::GetRenderer()
{
    return renderer_;
}

UViewPoint& URenderContext::GetViewPoint()
{
    return viewPoint_;
}

UViewport& URenderContext::GetViewport()
{
	return viewport_;
}

void URenderContext::DrawFrame()
{
	renderer_.DrawFrame({
		.view = viewPoint_.GetViewMatrix(),
		.proj = viewPoint_.GetProjectionMatrix(viewport_.GetAspectRatio()),
		.viewPos = viewPoint_.GetPosition(),
		.windowSize = viewport_.GetExtent(),
		.time = SClock::Now(),
		.fov = viewPoint_.GetFOV(),
		.aspectRatio = viewport_.GetAspectRatio(),
		.depthNear = viewPoint_.GetDepthNear(),
	});
}
