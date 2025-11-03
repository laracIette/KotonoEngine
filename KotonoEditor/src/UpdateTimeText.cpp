#include "UpdateTimeText.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/TimeManager.h>
#include <kotono_framework/Timer.h>
#include <kotono_interface/widgets.h>

WWidget* WUpdateTimeText::Build()
{
    Framework.TimeManager().GetTimer("update time text").EventCompleted()
        .AddListener(KtDelegate(this, &WUpdateTimeText::UpdateText));

    return new WText({
        .text = std::format("U {:.8f}s", Framework.TimeManager().AverageUpdateTime()),
        .fontSize = { 20.0f, 24.0f },
        .spacing = -6.0f,
    });
}

void WUpdateTimeText::Cleanup()
{
    Framework.TimeManager().GetTimer("update time text").EventCompleted()
        .RemoveListener(KtDelegate(this, &WUpdateTimeText::UpdateText));

    WWidget::Cleanup();
}

void WUpdateTimeText::UpdateText()
{
    SetState([]() {});
}
