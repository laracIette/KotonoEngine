#pragma once
#include "SceneObject.h"
#include <kotono_framework/Shader.h>
#include <kotono_framework/Model.h>
class TMesh : public TSceneObject
{
	BASECLASS(TSceneObject)

public:
	void Update() override;

	KtShader* GetShader() const;
	KtModel* GetModel() const;

	void SetShader(KtShader* shader);
	void SetModel(KtModel* model);

	void AddToRenderQueue() const;

private:
	KtShader* _shader;
	KtModel* _model;
};

