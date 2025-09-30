#include "UpdateTimeText.h"
#include <kotono_engine/Engine.h>
#include <kotono_engine/ObjectManager.h>
#include <kotono_engine/Timer.h>
#include <kotono_interface/widgets.h>

WWidget* WUpdateTimeText::Build()
{
    updateTimer_ = Engine.ObjectManager().Create<KTimer>();
    updateTimer_->GetEventCompleted().AddListener(KtDelegate(this, &WUpdateTimeText::UpdateText));
    updateTimer_->SetDuration(UDuration::FromSeconds(1.0f / 24.0f));
    updateTimer_->Start();
    return new WText({
        .text = std::format("U {:.8f}s", Engine.ObjectManager().GetAverageUpdateTime()),
        .fontSize = { 20.0f, 24.0f },
        .spacing = -6.0f,
    });
}

void WUpdateTimeText::Cleanup()
{
    updateTimer_->Delete();

    WWidget::Cleanup();
}

void WUpdateTimeText::UpdateText()
{
    SetState([]() {});
}
