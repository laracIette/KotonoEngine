#include "SceneMeshComponent.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ModelManager.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/Model.h>
#include <kotono_framework/Renderable3DProxy.h>
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

    isProxyDirty_ = true;

    spinTask_->SetDuration(5.0f);
    spinTask_->ListenEvent(Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED), 
        KtDelegate<>(spinTask_, &KTask::Start));
    
    ListenEvent(spinTask_->GetEventUpdate(), 
        KtDelegate<>(this, &KSceneMeshComponent::Spin));
    ListenEvent(Engine.GetObjectManager().GetEventDrawSceneObjects(), 
        KtDelegate<>(this, &KSceneMeshComponent::AddModelToRenderQueue));
    ListenEvent(Engine.GetObjectManager().GetEventDrawSceneObjectWireframes(), 
        KtDelegate<>(this, &KSceneMeshComponent::AddWireframeToRenderQueue));
}

void KSceneMeshComponent::Update()
{
    Base::Update();
}

void KSceneMeshComponent::Cleanup()
{
    Base::Cleanup();
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

void KSceneMeshComponent::AddModelToRenderQueue()
{
    if (isProxyDirty_)
    {
        const auto proxy = CreateProxy();
    }

    KtAddToRenderQueue3DArgs args{};
    args.Shader = shader_;
    args.Renderable = model_;
    args.Viewport = GetOwner()->GetViewport();
    args.ObjectData.Model = GetModelMatrix();
    Framework.GetRenderer().AddToRenderQueue3D(args);
}

void KSceneMeshComponent::AddWireframeToRenderQueue()
{
    KtAddToRenderQueue3DArgs args{};
    args.Shader = WireframeShader;
    args.Renderable = model_;
    args.Viewport = GetOwner()->GetViewport();
    args.ObjectData.Model = GetModelMatrix();
    Framework.GetRenderer().AddToRenderQueue3D(args);
}

const KtRenderable3DProxy KSceneMeshComponent::CreateProxy() const
{
    KtRenderable3DProxy proxy{};
    proxy.viewport = GetOwner()->GetViewport();
    proxy.modelMatrix = GetModelMatrix();
    proxy.shader = shader_;
    proxy.renderable = model_;
    return proxy;
}

void KSceneMeshComponent::Spin()
{
    const float speed = 10.0f * Engine.GetTime().GetDelta();
    const glm::quat rotation = glm::quat(glm::radians(glm::vec3(0.0f, speed, 0.0f)));
    Rotate(rotation);
}
