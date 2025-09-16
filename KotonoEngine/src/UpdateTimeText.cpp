#include "UpdateTimeText.h"
#include "Engine.h"
#include "Timer.h"
#include <kotono_interface/widgets.h>

WWidget* WUpdateTimeText::Build()
{
    updateTimer_ = Engine.GetObjectManager().Create<KTimer>();
    updateTimer_->GetEventCompleted().AddListener(KtDelegate(this, &WUpdateTimeText::UpdateText));
    updateTimer_->SetDuration(UDuration::FromSeconds(1.0f / 24.0f));
    updateTimer_->Start();
    return new WText({
        .text = std::format("U {:.8f}s", Engine.GetObjectManager().GetAverageUpdateTime()),
        .fontSize = { 50.0f, 60.0f },
        .spacing = 0.0f,
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
