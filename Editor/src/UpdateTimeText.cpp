#include "UpdateTimeText.h"

#include <kotono_core/Interface.h>
#include <kotono_core/Scene.h>
#include <kotono_interface/widgets.h>
#include <kotono_timing/Timer.h>
#include <kotono_timing/TimerManager.h>

WidgetPtr WUpdateTimeText::Build()
{
    text_ = UCreate<WText>{ "Update Time Text" }("Update Time Text");
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
    if (GetScene()->GetIsGamePlaying())
    {
        f32 const delta{ GetScene()->GetDeltaTime() };
        text_->SetText(std::format("S {0:.8f}FPS / {1:.8f}s", 1.0f / delta, delta));
    }
    else
    {
        f32 const delta{ GetInterface()->GetTimeContext().lastDelta };
        text_->SetText(std::format("I {0:.8f}FPS / {1:.8f}s", 1.0f / delta, delta));
    }
}

#include "generated/UpdateTimeText.generated.inl"
