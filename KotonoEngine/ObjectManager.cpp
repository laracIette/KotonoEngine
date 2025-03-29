#include "ObjectManager.h"
#include <kotono_framework/Framework.h>

void KObjectManager::Init()
{
	Framework.GetInputManager().GetKeyboard()
		.GetEvent(KT_KEY_ESCAPE, KT_INPUT_STATE_PRESSED)
		.AddListener(this, &KObjectManager::Quit);


	KtShader* shader2D = Framework.GetShaderManager().Get(
		Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2D.ktshader)"
	);
	KtShader* shader3D = Framework.GetShaderManager().Get(
		Framework.GetPath().GetFrameworkPath() / R"(shaders\shader3D.ktshader)"
	);
	shader2D->SetName("2D Shader");
	shader3D->SetName("3D Shader");

	KtModel* model1 = Framework.GetModelManager().Get(
		Framework.GetPath().GetSolutionPath() / (R"(assets\models\viking_room.obj)")
	);
	KtModel* model2 = Framework.GetModelManager().Get(
		Framework.GetPath().GetSolutionPath() / R"(assets\models\SM_Column_low.fbx)"
	);


	_mesh1.SetShader(shader3D);
	_mesh1.SetModel(model1);

	_mesh2.SetShader(shader3D);
	_mesh2.SetModel(model2);
	_mesh2.GetTransform().SetRelativeLocation(glm::vec3(2.0f, 0.0f, 0.0f));
	_mesh2.GetTransform().SetRelativeScale(glm::vec3(0.2f));

	_image1.SetShader(shader2D);
	_image1.GetRect().SetBaseSize(glm::uvec2(1024, 1024));
	_image1.GetRect().SetRelativeScale(glm::vec2(0.5f));
}

void KObjectManager::Update()
{
	WindowViewport.SetOffset({ 0, 0 });
	WindowViewport.SetExtent(Framework.GetRenderer().GetSwapChainExtent());

	static const auto startTime = std::chrono::high_resolution_clock::now();
	const auto currentTime = std::chrono::high_resolution_clock::now();
	const float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	const float uboTime = time / 10.0f;

	const VkExtent2D swapChainExtent = Framework.GetRenderer().GetSwapChainExtent();

	KtUniformData3D ubo{};
	ubo.View = glm::lookAt(glm::vec3(cos(uboTime) * 10.0f, sin(uboTime) * 10.0f, 3.0f), glm::vec3(cos(uboTime) * 5.0f, sin(uboTime) * 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.Projection = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 1000.0f);
	ubo.Projection[1][1] *= -1.0f;

	Framework.GetRenderer().GetRenderer3D().SetUniformData(ubo);

	_mesh1.AddToRenderQueue();
	_mesh2.AddToRenderQueue();
	_image1.AddToRenderQueue();
}

void KObjectManager::Cleanup()
{
}

void KObjectManager::Quit()
{
	Framework.GetWindow().Close();
}
