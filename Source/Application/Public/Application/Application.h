#pragma once

#include "Average.h"
#include "Containers/List.h"
#include "Context/Context.h"
#include "Device/Device.h"
#include "Clock/Clock.h"
#include "Timer/Timer.h"

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
	void DrawFrame() const;

	void LogUPS() const;

private:
	UContext context_;
	UDevice device_;

	UMainWindowContext* mainWindowContext_;
	UList<USecondaryWindowContext*> secondaryWindowContexts_;

	UClock clock_;
	f32 now_;
	UAverage<f32, 256> averageUpdateTime_;

	UTimer logUPSTimer_;
};
