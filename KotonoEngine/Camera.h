#pragma once
#include "Drawable3D.h"
#include "CameraView.h"
class Camera final :
	public Drawable3D
{
	BASECLASS(Drawable3D)

private:
	CameraView _view;
};

