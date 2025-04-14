#pragma once
#pragma once
#include "ModelWireframe.h"
#include "Viewport.h"
#include "ObjectData3D.h"
struct KtAddToWireframeQueue3DArgs
{
	KtModelWireframe* Model;
	KtViewport* Viewport;
	KtObjectData3D ObjectData;
};
