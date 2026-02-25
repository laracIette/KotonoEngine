#include "UpdateTimeText.h"
#include <kotono_timing/TimerManager.h>
#include <kotono_timing/Timer.h>
#include <kotono_core/TimeManager.h>
#include <kotono_interface/widgets.h>

WidgetPtr WUpdateTimeText::Build()
{
    TimerManager.GetTimer("update time text").EventCompleted().AddListener(this, &WUpdateTimeText::UpdateText);

    return new WText({
        .text = std::format("U {:.8f}s", TimeManager.AverageUpdateTime()),
        .fontSize = { 20.0f, 24.0f },
        .spacing = -6.0f,
    });
}

void WUpdateTimeText::Cleanup()
{
    TimerManager.GetTimer("update time text").EventCompleted().RemoveListener(this, &WUpdateTimeText::UpdateText);

    WWidget::Cleanup();
}

void WUpdateTimeText::UpdateText()
{
    SetState([]() {});
}
