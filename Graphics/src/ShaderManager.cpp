#include "ShaderManager.h"
#include "Shader.h"
#include "SpvCompiler.h"

void KtShaderManager::Init()
{
    KtSpvCompiler spvCompiler{};
    spvCompiler.CompileUpdated();
}

void KtShaderManager::Cleanup()
{
    for (auto& [path, shader] : shaders_)
    {
        shader->Cleanup();
        delete shader;
    }
    shaders_.clear();
}

KtShader* KtShaderManager::Get(const UPath& path)
{
    const auto it{ shaders_.find(path) };
    return it != shaders_.end() ? it->second : Create(path);
}

KtShader* KtShaderManager::Create(const UPath& path)
{
    auto* shader{ new KtShader(path) };
    shader->Init();
    shaders_[path] = shader;
    return shader;
}
