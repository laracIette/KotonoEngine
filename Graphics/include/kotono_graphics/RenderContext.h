#pragma once
#include "Renderer.h"
#include "ViewPoint.h"
#include "Viewport.h"
class URenderContext final
{
public:
	void Init();
	void Cleanup();

	URenderer& GetRenderer();
	UViewPoint& GetViewPoint();
	UViewport& GetViewport();

	void DrawFrame();

private:
	URenderer renderer_;
	UViewPoint viewPoint_;
	UViewport viewport_;
};

inline URenderContext RenderContext;
