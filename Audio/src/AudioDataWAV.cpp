#include "AudioDataWAV.h"

UAudioDataWAV::UAudioDataWAV(UPath const& path)
{
    drwav_uint64 totalFrameCount;

    pSampleData = drwav_open_file_and_read_pcm_frames_s16(
        path.ToPath().string().c_str(), &channels, &sampleRate, &totalFrameCount, NULL
    );

    if (!pSampleData)
    {
        throw std::runtime_error{ "failed to load file" };
    }

    dataSize = totalFrameCount * channels * sizeof(drwav_int16);
}

UAudioDataWAV::~UAudioDataWAV()
{
    drwav_free(pSampleData, NULL);
}

