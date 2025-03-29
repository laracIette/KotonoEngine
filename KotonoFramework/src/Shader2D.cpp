#include "Shader2D.h"
#include "Framework.h"

void KtShader2D::Init()
{
	SetName("2D Shader");
	SetShaderPath(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2D.ktshader)");
	KtShader::Init();
}
