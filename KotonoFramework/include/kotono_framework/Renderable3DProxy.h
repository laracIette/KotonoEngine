#pragma once
#include "glm_includes.h"
class KtShader;
class KtViewport;
class KtRenderable3D;
struct KtRenderable3DProxy final
{
public:
	KtShader* shader;
	KtRenderable3D* renderable;
	KtViewport* viewport;
	glm::mat4 modelMatrix;

    bool isDirty;
};

