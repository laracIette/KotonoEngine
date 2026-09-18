#pragma once
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/Average.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/Device.h>
#include <kotono_timing/Timer.h>
#include <vector>
class UMainWindowContext;
class USecondaryWindowContext;
class UApplication final
{
public:
	UApplication();

	void Run();

private:
	void Init();
	void Cleanup();

	void Update();
	void DrawFrame();

	void LogUPS() const;

private:
	UContext context_;
	UDevice device_;

	UMainWindowContext* mainWindowContext_;
	std::vector<USecondaryWindowContext*> secondaryWindowContexts_;

	f32 now_;
	f32 deltaTime_;
	UAverage<f32, 256> averageUpdateTime_;

	UTimer logUPSTimer_;
};
