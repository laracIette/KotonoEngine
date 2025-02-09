#pragma once
#include <unordered_map>
#include <unordered_set>
#include "Shader.h"
#include "Model.h"
class KtRenderQueue3D final
{
public:
	void Add(KtShader* shader, KtModel* model);

private:
	std::unordered_map<KtShader*, std::unordered_set<KtModel*>> _queue; // add ubo?
};

