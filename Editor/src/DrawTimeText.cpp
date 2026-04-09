#include "DrawTimeText.h"
#include <kotono_timing/TimerManager.h>
#include <kotono_timing/Timer.h>
#include <kotono_core/TimeManager.h>
#include <kotono_interface/widgets.h>

WidgetPtr WDrawTimeText::Build()
{
    TimerManager.GetTimer("update time text").EventCompleted().AddListener(this, &WDrawTimeText::UpdateText);

    return new WText({
        .text = std::format("D {:.8f}s", TimeManager.AverageRenderTime()),
        .fontSize = { 20.0f, 24.0f },
        .spacing = -6.0f,
    });
}

void WDrawTimeText::Cleanup()
{
    TimerManager.GetTimer("update time text").EventCompleted().RemoveListener(this, &WDrawTimeText::UpdateText);

    Base::Cleanup();
}

void WDrawTimeText::UpdateText()
{
    SetState([]() {});
}

#include "generated/DrawTimeText.generated.inl"
