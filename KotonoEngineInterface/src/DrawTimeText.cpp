#include "DrawTimeText.h"
#include <kotono_engine/Engine.h>
#include <kotono_engine/TimeManager.h>
#include <kotono_engine/ObjectManager.h>
#include <kotono_engine/Timer.h>
#include <kotono_interface/widgets.h>

WWidget* WDrawTimeText::Build()
{
    updateTimer_ = Engine.ObjectManager().Create<KTimer>();
    updateTimer_->GetEventCompleted().AddListener(KtDelegate(this, &WDrawTimeText::UpdateText));
    updateTimer_->SetDuration(UDuration::FromSeconds(1.0f / 24.0f));
    updateTimer_->Start();
    return new WText({
        .text = std::format("D {:.8f}s", Engine.TimeManager().AverageRenderTime()),
        .fontSize = { 20.0f, 24.0f },
        .spacing = -6.0f,
    });
}

void WDrawTimeText::Cleanup()
{
    updateTimer_->Delete();

    WWidget::Cleanup();
}

void WDrawTimeText::UpdateText()
{
    SetState([]() {});
}