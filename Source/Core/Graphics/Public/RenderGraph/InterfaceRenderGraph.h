#pragma once

#include "Containers/List.h"
#include "InterfaceDrawData.h"

struct UInterfaceRenderGraph final
{
	UList<UInterfaceDrawData> drawDatas;
};
