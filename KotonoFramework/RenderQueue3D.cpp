#include "RenderQueue3D.h"

void KtRenderQueue3D::Add(KtShader* shader, KtModel* model)
{
	if (_queue.contains(shader))
	{
		_queue[shader].insert(model);
	}
	else
	{
		_queue[shader] = { model };
	}
}
