#include "UpdateTimeText.h"

#include <cassert>
#include <kotono_core/Interface.h>
#include <kotono_interface/widgets.h>

WidgetPtr WUpdateTimeText::Build()
{
    UPtr text{ UCreate<WText>{ "Update Time Text" }("Update Time Text") };
    text->SetText([this]() { 
        assert(this);
        f32 const delta{ GetInterface()->GetDeltaTime() };
        return std::format("{0:.8f}FPS / {1:.8f}s", 1.0f / delta, delta); 
    });
    return text;
}

#include "generated/UpdateTimeText.generated.inl"
