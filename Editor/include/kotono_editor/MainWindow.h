#pragma once
#include "generated/MainWindow.generated.h"
#include <assert.h>
#include <kotono_object/SceneContext.h>
class WMainWindow final : public WSceneContext
{
	GENERATED_WMAINWINDOW()

protected:
	WidgetPtr Build() override;

public:
	void SetInterface(UInterface* newInterface) { interface_ = newInterface; }
	UInterface* GetInterface() const override { assert(interface_); return interface_; }

private:
	UInterface* interface_;
};
