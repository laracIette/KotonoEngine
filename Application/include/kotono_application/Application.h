#pragma once
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/Average.h>
#include <kotono_common/Handle.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/Device.h>
#include <kotono_platform/Surface.h>
#include <kotono_platform/Window.h>
#include <kotono_rendering/Renderer.h>
#include <span>
#include <unordered_map>
#include <vector>
#ifdef EDITOR
#include <kotono_core/Ptr.h>
#endif
class UInterface;
#ifdef EDITOR
class WMainWindow;
#endif
class UApplication final
{
public:
	UApplication();

	void Run();

private:
	void Init();
	void Update();
	void Cleanup();

	void LogUPS() const;
	void OnWindowResized(glm::uvec2 const& extent);

private:
	UWindow window_;
	UContext context_;
	USurface surface_;
	UDevice device_;
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
