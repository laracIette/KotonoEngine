#pragma once
#include <kotono_framework/Shader.h>
#include <kotono_framework/Model.h>
class KObjectManager
{
public:
	void Init();
	void Update();
	void Cleanup();

private:
	void Quit();

	KtShader* _shader2D;
	KtShader* _shader3D;

	KtModel* _model1;
	KtModel* _model2;
};

