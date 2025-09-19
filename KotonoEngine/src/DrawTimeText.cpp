#include "DrawTimeText.h"
#include "Engine.h"
#include "Timer.h"
#include <kotono_interface/widgets.h>

WWidget* WDrawTimeText::Build()
{
    updateTimer_ = Engine.GetObjectManager().Create<KTimer>();
    updateTimer_->GetEventCompleted().AddListener(KtDelegate(this, &WDrawTimeText::UpdateText));
    updateTimer_->SetDuration(UDuration::FromSeconds(1.0f / 24.0f));
    updateTimer_->Start();
    return new WText({
        .text = std::format("D {:.8f}s", Engine.GetObjectManager().GetAverageDrawTime()),
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