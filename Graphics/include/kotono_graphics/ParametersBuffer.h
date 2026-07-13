#pragma once
#include "frames_in_flight.h"
#include "ParametersBufferData.h"
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <vector>
#include <vulkan/vulkan_core.h>
class GParametersBuffer final
{
    using Data = UParametersBufferData;

public:
    struct FrameData
    {
        UAllocatedBuffer dataBuffer;
    };

public:
    void Init();
    void Cleanup() const;

    Data* RegisterParameters();
    void UnregisterParameters(Data* slot);
    u32 GetIndex(const Data* slot) const;

    void UpdateBuffer(const u32 frameIndex);
    VkDeviceAddress GetAddress(const u32 frameIndex) const;

private:
    void CreateBuffers();
    Data* FindParametersSlot();

private:
    UFramesInFlightArray<FrameData> frameDatas_;

    std::vector<Data> datas_;
    std::vector<Data*> freeDataSlots_;
    u32 dataCount_;
};

inline GParametersBuffer ParametersBuffer;
