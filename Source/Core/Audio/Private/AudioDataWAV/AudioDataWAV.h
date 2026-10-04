#pragma once
#include <dr_libs/dr_wav.h>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
struct UAudioDataWAV final
{
	UAudioDataWAV(UPath const& path);
	~UAudioDataWAV();

	drwav_int16* pSampleData;
	u32 channels;
	u32 sampleRate;
	size dataSize;
};