#pragma once
#include <vector>
//#include "Framebuffer.h"
#include "Drawable2D.h"
#include "Drawable3D.h"
class Renderer final
{
public:
	void AddToRenderQueue(Drawable* drawable);
	void DrawRenderQueue();

	void ResizeFramebuffer() const;

private:
	//Framebuffer _framebuffer;

	std::vector<Drawable2D*> _drawable2DRenderQueue;
	std::vector<Drawable3D*> _drawable3DOpaqueRenderQueue;
	std::vector<Drawable3D*> _drawable3DTransparentRenderQueue;
	std::vector<Drawable3D*> _drawable3DFrontRenderQueue;

	void DrawDrawable2DRenderQueue() const;
	void DrawDrawable3DOpaqueRenderQueue() const;
	void DrawDrawable3DTransparentRenderQueue() const;
	void DrawDrawable3DFrontRenderQueue() const;

	void SortDrawable2DRenderQueue();
	void SortDrawable3DTransparentRenderQueue();

	void DrawDrawable(Drawable* drawable) const;
};