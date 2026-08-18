#include "UpdateTimeText.h"

#include <kotono_interface/widgets.h>
#include <kotono_object/Scene.h>
#include <kotono_timing/Timer.h>
#include <kotono_timing/TimerManager.h>

WidgetPtr WUpdateTimeText::Build()
{
    text_ = UCreate<WText>{}();
    text_->SetName("Update Time Text");
    text_->SetText("Update Time Text");
    text_->SetFontSize({ 20.0f, 24.0f });
    text_->SetSpacing(-6.0f);
    return text_;
}

void WUpdateTimeText::Display(UWidgetDisplaySettings const& displaySettings)
{
    Base::Display(displaySettings);

    TimerManager.GetTimer("update time text").EventCompleted().AddListener(this, &Self::UpdateText);
}

void WUpdateTimeText::Remove()
{
    Base::Remove();

    TimerManager.GetTimer("update time text").EventCompleted().RemoveListener(this, &Self::UpdateText);
}

void WUpdateTimeText::UpdateText() const
{
    text_->SetText(std::format("U {0:.8f}s", GetScene()->GetGameTime().lastDelta));
}

#include "generated/UpdateTimeText.generated.inl"
