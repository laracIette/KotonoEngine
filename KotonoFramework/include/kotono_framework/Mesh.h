#pragma once
#include "Model.h"
#include "Shader.h"
#include "max_frames_in_flight.h"
class KtMesh final
{
public:
	void AddToRenderQueue3D(const glm::mat4& modelMatrix) const;

	KtShader* GetShader() const;
	KtModel* GetModel() const;

	void SetShader(KtShader* shader);
	void SetModel(KtModel* model);

private:
	KtShader* _shader;
	KtModel* _model;
};

