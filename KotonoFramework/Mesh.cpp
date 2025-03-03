#include "Mesh.h"
#include "Framework.h"
#include "log.h"

void KtMesh::AddToRenderQueue() const
{
    static const auto startTime = std::chrono::high_resolution_clock::now();
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    Framework.GetWindow().GetRenderer().AddToRenderQueue(
        _shader,
        _model,
        {
            glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f))
        }
    );
}

KtShader* KtMesh::GetShader() const
{
    return _shader;
}

KtModel* KtMesh::GetModel() const
{
    return _model;
}

void KtMesh::SetShader(KtShader* shader)
{
    _shader = shader;
}

void KtMesh::SetModel(KtModel* model)
{
    _model = model;
}

const glm::mat4 KtMesh::GetModelMatrix() const
{
    static const auto startTime = std::chrono::high_resolution_clock::now();

    const auto currentTime = std::chrono::high_resolution_clock::now();
    const float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    return glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}
