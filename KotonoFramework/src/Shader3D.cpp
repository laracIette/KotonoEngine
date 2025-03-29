#include "Shader3D.h"
#include <vulkan/vulkan.h>
#include <algorithm>
#include "Vertex3D.h"
#include "File.h"
#include "Framework.h"
#include "log.h"
#include "vk_utils.h"

void KtShader3D::Init()
{
	SetName("3D Shader");
	SetVertPath(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader3DVert.spv)");
	SetFragPath(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader3DFrag.spv)");
	SetShaderPath(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader3D.ktshader)");
	KtShader::Init();
}