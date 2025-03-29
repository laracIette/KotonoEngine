#include "Shader3D.h"
#include "Framework.h"

void KtShader3D::Init()
{
	SetName("3D Shader");
	SetShaderPath(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader3D.ktshader)");
	KtShader::Init();
}