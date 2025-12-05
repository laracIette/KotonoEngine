#include "DrawTimeText.h"
#include <kotono_framework/Framework.h>
#include <kotono_timing/TimeManager.h>
#include <kotono_timing/Timer.h>
#include <kotono_interface/widgets.h>

WWidget* WDrawTimeText::Build()
{
    Framework.TimeManager().GetTimer("update time text").EventCompleted()
        .AddListener(KtDelegate(this, &WDrawTimeText::UpdateText));

    return new WText({
        .text = std::format("D {:.8f}s", 0.0f/*Framework.TimeManager().AverageRenderTime()*/), // todo: move to framework
        .fontSize = { 20.0f, 24.0f },
        .spacing = -6.0f,
    });
}

void WDrawTimeText::Cleanup()
{
    Framework.TimeManager().GetTimer("update time text").EventCompleted()
        .RemoveListener(KtDelegate(this, &WDrawTimeText::UpdateText));

    WWidget::Cleanup();
}

void WDrawTimeText::UpdateText()
{
    SetState([]() {});
}