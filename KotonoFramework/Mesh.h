#pragma once
#include "Model.h"
#include "Shader.h"
class KtMesh final
{
public:
	void AddToRenderQueue() const;

	KtModel* GetModel() const;
	KtShader* GetShader() const;

	void SetModel(KtModel* model);
	void SetShader(KtShader* shader);

private:
	KtModel* _model;
	KtShader* _shader;
};

