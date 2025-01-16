#include "EICViewport.h"

EICViewport::EICViewport() :
	Base("Viewport"),
	_cameraView(nullptr)
{
}

void EICViewport::OnResize()
{
	/*float aspectRatio = GetRect().GetSize().x / GetRect().GetSize().y;
	_cameraView->SetAspectRatio(aspectRatio);*/
}

void EICViewport::Draw() const
{
}
