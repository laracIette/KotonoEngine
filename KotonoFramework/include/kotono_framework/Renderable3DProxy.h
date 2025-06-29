#pragma once
#include "glm_includes.h"
class KtShader;
class KtViewport;
class KtRenderable3D;
struct KtRenderable3DProxy final
{
	KtShader* shader;
	KtViewport* viewport;
	KtRenderable3D* renderable;
	glm::mat4 modelMatrix;
};

namespace std
{
    template<>
    struct hash<KtRenderable3DProxy>
    {
        size_t operator()(const KtRenderable3DProxy& proxy) const
        {
            size_t h = 0;
            h ^= hash<KtShader*>()(proxy.shader) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<KtViewport*>()(proxy.viewport) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<KtRenderable3D*>()(proxy.renderable) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<glm::mat4>()(proxy.modelMatrix) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
}

