#include "Renderer.h"
#include "KotonoEngine.h"
#include <algorithm>

void Renderer::AddToRenderQueue(Drawable* drawable)
{
	if (!drawable->GetColor().IsVisible())
	{
		return;
	}

	if (auto drawable2D = dynamic_cast<Drawable2D*>(drawable))
	{
		glm::vec2 windowSize = Engine.GetWindow().GetSize();
		glm::vec2 windowPosition = windowSize / 2.0f;

		if (drawable2D->GetRect()->Overlaps(windowPosition, windowSize))
		{
			_drawable2DRenderQueue.push_back(drawable2D);
		}
	}
	else if (auto drawable3D = dynamic_cast<Drawable3D*>(drawable))
	{
		if (drawable3D->GetIsFront())
		{
			_drawable3DFrontRenderQueue.push_back(drawable3D);
		}
		else if (drawable3D->GetColor().IsOpaque())
		{
			_drawable3DOpaqueRenderQueue.push_back(drawable3D);
		}
		else
		{
			_drawable3DTransparentRenderQueue.push_back(drawable3D);
		}
	}
}

void Renderer::DrawRenderQueue()
{
	SortDrawable2DRenderQueue();
	SortDrawable3DTransparentRenderQueue();

	//_framebuffer.BeginDraw();

	DrawDrawable2DRenderQueue(); // TODO: no transparency
	DrawDrawable3DOpaqueRenderQueue();
	DrawDrawable3DTransparentRenderQueue();
	DrawDrawable3DFrontRenderQueue();

	//_framebuffer.DrawTextures();
}

void Renderer::ResizeFramebuffer() const
{
	//_framebuffer.ResizeTextures();
}

void Renderer::DrawDrawable(Drawable* drawable) const
{
	drawable->UpdateShader();
	drawable->Draw();
}

void Renderer::DrawDrawable2DRenderQueue() const
{
	if (_drawable2DRenderQueue.empty())
	{
		return;
	}

	// Enable blending and set blend function
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw each drawable drawable in the queue
	for (auto* drawable : _drawable2DRenderQueue)
	{
		DrawDrawable(drawable);
	}

	// Disable blending
	glDisable(GL_BLEND);
}

void Renderer::DrawDrawable3DOpaqueRenderQueue() const
{
	if (_drawable3DOpaqueRenderQueue.empty())
	{
		return;
	}

	// Set depth function to less-than-or-equal and enable depth testing
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	// Draw each drawable drawable in the queue
	for (auto* drawable : _drawable3DOpaqueRenderQueue)
	{
		DrawDrawable(drawable);
	}

	// Restore depth function to default (less than) and disable depth testing
	glDepthFunc(GL_LESS);
	glDisable(GL_DEPTH_TEST);
}

void Renderer::DrawDrawable3DTransparentRenderQueue() const
{
	if (_drawable3DTransparentRenderQueue.empty())
	{
		return;
	}

	// Set up transparency and depth options
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	// Draw each drawable drawable in the queue
	for (auto* drawable : _drawable3DTransparentRenderQueue)
	{
		DrawDrawable(drawable);
	}

	// Restore depth mask and disable blending and depth test
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

void Renderer::DrawDrawable3DFrontRenderQueue() const
{
	if (_drawable3DFrontRenderQueue.empty())
	{
		return;
	}

	// Draw each drawable drawable in the queue
	for (auto* drawable : _drawable3DFrontRenderQueue)
	{
		DrawDrawable(drawable);
	}
}

void Renderer::SortDrawable2DRenderQueue()
{
	// Sort by layer from lowest to highest
	std::sort(
		_drawable2DRenderQueue.begin(), _drawable2DRenderQueue.end(),
		[](const Drawable2D* a, const Drawable2D* b)
		{
			return a->GetLayer() < b->GetLayer();
		}
	);
}

void Renderer::SortDrawable3DTransparentRenderQueue()
{
	// Sort by distance to camera from furthest to closest
	std::sort(
		_drawable3DTransparentRenderQueue.begin(), _drawable3DTransparentRenderQueue.end(),
		[](const Drawable3D* a, const Drawable3D* b)
		{
			Transform* cameraTransform = nullptr;

			float distanceA = a->GetTransform()->GetDistance(cameraTransform);
			float distanceB = b->GetTransform()->GetDistance(cameraTransform);
			return distanceA > distanceB;
		}
	);
}
