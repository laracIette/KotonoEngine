#include "SceneMeshComponent.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ModelManager.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/Model.h>
#include <kotono_framework/log.h>
#include <nlohmann/json.hpp>
#include "Engine.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "Task.h"
#include "Time.h"

static KtShader* WireframeShader = nullptr;

void KSceneMeshComponent::Construct()
{
    Base::Construct();

    spinTask_ = AddObject<KTask>();

    if (!WireframeShader)
    {
        const auto path = Framework.GetPath().GetFrameworkPath() / R"(shaders\wireframe3D.ktshader)";
        WireframeShader = Framework.GetShaderManager().Get(path);
    }
}

void KSceneMeshComponent::Init()
{
    Base::Init();

    InitProxy();
    InitSpin();
}

void KSceneMeshComponent::InitProxy()
{
    CreateProxy();
    Framework.GetRenderer().AddToRenderQueue3D(&proxy_);
    const KtDelegate<> transformDelegate(this, &KSceneMeshComponent::MarkProxyTransformDirty);
    ListenEvent(GetEventUpdateTransform(), transformDelegate);
}

void KSceneMeshComponent::InitSpin()
{
    const KtDelegate<> startSpinDelegate(spinTask_, &KTask::Start);
    const KtDelegate<> spinMeshDelegate(this, &KSceneMeshComponent::Spin);
    spinTask_->SetDuration(5.0f);
    spinTask_->ListenEvent(Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED), startSpinDelegate);
    ListenEvent(spinTask_->GetEventUpdate(), spinMeshDelegate);
}

void KSceneMeshComponent::Update()
{
    Base::Update();
}

void KSceneMeshComponent::Cleanup()
{
    Base::Cleanup();
    Framework.GetRenderer().RemoveFromRenderQueue3D(&proxy_);
}

KtShader* KSceneMeshComponent::GetShader() const
{
    return shader_;
}

KtModel* KSceneMeshComponent::GetModel() const
{
    return model_;
}

void KSceneMeshComponent::SetShader(KtShader* shader)
{
    shader_ = shader;
}

void KSceneMeshComponent::SetModel(KtModel* model)
{
    model_ = model;
}

void KSceneMeshComponent::SerializeTo(nlohmann::json& json) const
{
    Base::SerializeTo(json);
    json["shader"] = shader_ ? shader_->GetPath() : "";
    json["model"] = model_ ? model_->GetPath() : "";
}

void KSceneMeshComponent::DeserializeFrom(const nlohmann::json& json)
{
    Base::DeserializeFrom(json);
    shader_ = Framework.GetShaderManager().Get(json["shader"]);
    model_ = Framework.GetModelManager().Get(json["model"]);
}

void KSceneMeshComponent::CreateProxy()
{
    proxy_.viewport = GetOwner()->GetViewport();
    UpdateProxyModelMatrix();
    UpdateProxyShader();
    UpdateProxyRenderable();
}

void KSceneMeshComponent::UpdateProxyModelMatrix()
{
    proxy_.modelMatrix = GetModelMatrix();
}

void KSceneMeshComponent::UpdateProxyShader()
{
    proxy_.shader = shader_;
}

void KSceneMeshComponent::UpdateProxyRenderable()
{
    proxy_.renderable = model_;
}

void KSceneMeshComponent::MarkProxyTransformDirty()
{
    proxy_.isDirty = true;
    UpdateProxyModelMatrix();
}

void KSceneMeshComponent::Spin()
{
    const float speed = 10.0f * Engine.GetTime().GetDelta();
    const glm::quat rotation = glm::quat(glm::radians(glm::vec3(0.0f, speed, 0.0f)));
    Rotate(rotation);
}
