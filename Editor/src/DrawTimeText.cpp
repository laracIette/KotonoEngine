#include "DrawTimeText.h"
#include <kotono_timing/TimerManager.h>
#include <kotono_timing/Timer.h>
#include <kotono_core/TimeManager.h>
#include <kotono_interface/widgets.h>

WidgetPtr WDrawTimeText::Build()
{
    text_ = UCreate<WText>{}();
    text_->SetName("Draw Time Text");
    text_->SetText("Draw Time Text");
    text_->SetFontSize({ 20.0f, 24.0f });
    text_->SetSpacing(-6.0f);

    return text_;
}

void WDrawTimeText::Display(UWidgetDisplaySettings displaySettings)
{
    Base::Display(displaySettings);

    TimerManager.GetTimer("update time text").EventCompleted().AddListener(this, &Self::UpdateText);
}

void WDrawTimeText::Remove()
{
    Base::Remove();

    TimerManager.GetTimer("update time text").EventCompleted().RemoveListener(this, &Self::UpdateText);
}

void WDrawTimeText::UpdateText() const
{
    text_->SetText(std::format("D {:.8f}s", TimeManager.AverageRenderTime()));
}

#include "generated/DrawTimeText.generated.inl"
