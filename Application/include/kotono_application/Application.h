#pragma once
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/Average.h>
#include <kotono_common/Handle.h>
#include <kotono_rendering/Renderer.h>
#include <span>
#include <unordered_map>
#include <vector>
#ifdef EDITOR
#include <kotono_object/Ptr.h>
#endif
class UInterface;
#ifdef EDITOR
class WMainWindow;
#endif
class UApplication final
{
public:
	void Run();

private:
	void Init();
	void Update();
	void Cleanup();

	void LogUPS() const;
	void OnWindowResized(glm::uvec2 const& extent);

private:
	URenderer renderer_;
	UInterface* interface_;

	std::unordered_map<EHandle, u32> textureHandles_;
	std::unordered_map<EHandle, u32> sceneRenders_;

	f32 now_;
	f32 deltaTime_;
	UAverage<f32, 256> averageUpdateTime_;

#	ifdef EDITOR
	UPtr<WMainWindow> mainWindow_;
#	endif
};
