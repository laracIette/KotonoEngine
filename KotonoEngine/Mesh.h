#pragma once
#include "SceneObject.h"
#include <kotono_framework/Shader3D.h>
#include <kotono_framework/Model.h>
class TMesh : public TSceneObject
{
	BASECLASS(TSceneObject)

public:
	KtShader3D* GetShader() const;
	KtModel* GetModel() const;

	void SetShader(KtShader3D* shader);
	void SetModel(KtModel* model);


	void Draw() const;

private:
	KtShader3D* _shader;
	KtModel* _model;
};

