#include "ObjectManager.h"
#include <kotono_framework/Framework.h>

void KObjectManager::Init()
{
	Framework.GetInputManager().GetKeyboard()
		.GetEvent(KT_KEY_ESCAPE, KT_INPUT_STATE_PRESSED)
		.AddListener(this, &KObjectManager::Quit);


	_shader2D = Framework.GetShaderManager().Get(
		Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2D.ktshader)"
	);
	_shader2D->SetName("2D Shader");

	_shader3D = Framework.GetShaderManager().Get(
		Framework.GetPath().GetFrameworkPath() / R"(shaders\shader3D.ktshader)"
	);
	_shader3D->SetName("3D Shader");

	_model1 = Framework.GetModelManager().Get(
		Framework.GetPath().GetSolutionPath() / (R"(assets\models\viking_room.obj)")
	);
	_model2 = Framework.GetModelManager().Get(
		Framework.GetPath().GetSolutionPath() / R"(assets\models\SM_Column_low.fbx)"
	);
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

	glm::mat4 modelMatrix = glm::identity<glm::mat4>();
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f);
	float angle = time * glm::radians(90.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	modelMatrix = glm::identity<glm::mat4>();
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, angle, axis);
	modelMatrix = glm::scale(modelMatrix, scale);

	KtAddToRenderQueue3DArgs model1Args{};
	model1Args.Shader = _shader3D;
	model1Args.Model = _model1;
	model1Args.Viewport = &WindowViewport;
	model1Args.ObjectData = { modelMatrix };
	Framework.GetRenderer().GetRenderer3D().AddToRenderQueue(model1Args);

	KtAddToRenderQueue3DArgs model2Args{};
	model2Args.Shader = _shader3D;
	model2Args.Model = _model2;
	model2Args.Viewport = &WindowViewport;

	for (uint32_t i = 0; i < 1000; i++)
	{
		position = glm::vec3(cos(i), sin(i), 0.0f) * 0.01f * static_cast<float>(i);
		scale = glm::vec3(0.1f);

		modelMatrix = glm::identity<glm::mat4>();
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, angle, axis);
		modelMatrix = glm::scale(modelMatrix, scale);

		model2Args.ObjectData = { modelMatrix };
		Framework.GetRenderer().GetRenderer3D().AddToRenderQueue(model2Args);
	}

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	angle = 0.0f;
	scale = glm::vec3(1.0f, 1.0f, 1.0f);

	modelMatrix = glm::identity<glm::mat4>();
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, angle, axis);
	modelMatrix = glm::scale(modelMatrix, scale);

	KtAddToRenderQueue2DArgs imageArgs{};
	imageArgs.Shader = _shader2D;
	imageArgs.Viewport = &WindowViewport;
	imageArgs.ObjectData = { modelMatrix };
	Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(imageArgs);
}

void KObjectManager::Cleanup()
{
}

void KObjectManager::Quit()
{
	Framework.GetWindow().Close();
}
