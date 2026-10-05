#include "UpdateTimeText.h"

#include <Interface/Interface.h>
#include <core_widgets.h>

WidgetPtr WUpdateTimeText::Build()
{
    return UCreate<WText>{ "Update Time Text" }()
    | Apply(&WText::SetText, [this]() {
        f32 const delta{ GetInterface()->GetDeltaTime() };
        return std::format("{0:.8f}FPS / {1:.8f}s", 1.0f / delta, delta);
    })
    ;
}

#include "generated/UpdateTimeText.generated.inl"
