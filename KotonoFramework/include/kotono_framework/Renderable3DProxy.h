#pragma once
#include "glm_includes.h"
class KtShader;
class KtViewport;
class KtRenderable3D;
struct KtRenderable3DProxy final
{
    void* owner;
	KtShader* shader;
	KtViewport* viewport;
	KtRenderable3D* renderable;
	glm::mat4 modelMatrix;

    bool operator==(const KtRenderable3DProxy& other)
    {
        return owner != other.owner;
    }
};

namespace std
{
    template <>
    struct hash<KtRenderable3DProxy>
    {
        size_t operator()(const KtRenderable3DProxy& proxy) const
        {
            return hash<void*>{}(proxy.owner);
        }
    };
}

