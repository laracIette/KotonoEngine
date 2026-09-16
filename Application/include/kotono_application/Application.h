#pragma once
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/Average.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/Device.h>
#include <kotono_platform/Surface.h>
#include <kotono_platform/Window.h>
#include <kotono_rendering/Renderer.h>
#include <kotono_timing/Timer.h>
#ifdef EDITOR
#include <kotono_core/Ptr.h>
#endif
class UInterface;
#ifdef EDITOR
class WInterfaceRoot;
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

	f32 now_;
	f32 deltaTime_;
	UAverage<f32, 256> averageUpdateTime_;

	UTimer logUPSTimer_;

#	ifdef EDITOR
	UPtr<WInterfaceRoot> mainWindow_;
#	endif
};
