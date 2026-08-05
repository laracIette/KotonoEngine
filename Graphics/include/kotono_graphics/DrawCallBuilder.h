#pragma once
#include <kotono_common/types.h>
struct UDrawCall;
struct UDrawDataBufferData;
struct UTransformBufferData;
struct UParametersBufferData;
class URenderer;
enum class ERenderBucket : u8
{
	Opaque,
	Transparent,
	Interface,
};
struct UDrawCallBuilder final
{
public:
	UDrawCallBuilder();
	~UDrawCallBuilder();

	void Register(URenderer& renderer, const ERenderBucket renderBucket);
	void Unregister(URenderer& renderer);

	UDrawCall* GetDrawCall() const;
	UDrawDataBufferData* GetDrawData() const;
	UTransformBufferData* GetTransform() const;
	UParametersBufferData* GetParameters() const;

private:
	UDrawCall* drawCall_;
	bool isRegistered_;
	ERenderBucket renderBucket_;

	UDrawDataBufferData* drawData_;
	UTransformBufferData* transform_;
	UParametersBufferData* parameters_;
};