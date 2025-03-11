#pragma once
#include "Model.h"
#include "Shader3D.h"
#include "frames_in_flight.h"
class KtMesh final
{
public:
	void AddToRenderQueue3D(const glm::mat4& modelMatrix) const;

	KtShader3D* GetShader() const;
	KtModel* GetModel() const;

	void SetShader(KtShader3D* shader);
	void SetModel(KtModel* model);

private:
	KtShader3D* _shader;
	KtModel* _model;
};

