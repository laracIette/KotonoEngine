#include "Shader2D.h"
#include <vulkan/vulkan.h>
#include "Vertex2D.h"
#include "File.h"
#include "Framework.h"
#include "log.h"
#include "vk_utils.h"

void KtShader2D::Init()
{
	SetName("2D Shader");
	SetVertPath(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2DVert.spv)");
	SetFragPath(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2DFrag.spv)");
	SetShaderPath(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2D.ktshader)");
	KtShader::Init();
}
