#include "DrawCallBuilder.h"
#include <kotono_graphics/DrawCall.h>
#include <kotono_graphics/DrawDataBuffer.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/TransformBuffer.h>
#include <kotono_graphics/ParametersBuffer.h>

UDrawCallBuilder::UDrawCallBuilder() 
	: drawCall_{ new UDrawCall{} }
	, isRegistered_{ false }
	, drawData_{ DrawDataBuffer.RegisterDrawData() }
	, transform_{ TransformBuffer.RegisterTransform() }
	, parameters_{ ParametersBuffer.RegisterParameters() }
{
	drawCall_->index = DrawDataBuffer.GetIndex(drawData_);
	drawData_->transformIndex = TransformBuffer.GetIndex(transform_);
	drawData_->parametersIndex = ParametersBuffer.GetIndex(parameters_);
}

UDrawCallBuilder::~UDrawCallBuilder()
{
	DrawDataBuffer.UnregisterDrawData(drawData_);
	TransformBuffer.UnregisterTransform(transform_);
	ParametersBuffer.UnregisterParameters(parameters_);
	Unregister();
	delete drawCall_;
}

void UDrawCallBuilder::Register(const ERenderBucket renderBucket)
{
	if (isRegistered_)
	{
		return;
	}

	isRegistered_ = true;
	renderBucket_ = renderBucket;

	switch (renderBucket_)
	{
	case ERenderBucket::Opaque:			return Renderer.RegisterOpaqueDrawCall(drawCall_);
	case ERenderBucket::Transparent:	return;
	case ERenderBucket::Interface:		return;
	}
}

void UDrawCallBuilder::Unregister()
{
	if (!isRegistered_)
	{
		return;
	}

	isRegistered_ = false;

	switch (renderBucket_)
	{
	case ERenderBucket::Opaque:			return Renderer.UnregisterOpaqueDrawCall(drawCall_);
	case ERenderBucket::Transparent:	return;
	case ERenderBucket::Interface:		return;
	}
}

UDrawCall* UDrawCallBuilder::GetDrawCall() const
{
	return drawCall_;
}

UDrawDataBufferData* UDrawCallBuilder::GetDrawData() const
{
	return drawData_;
}

UTransformBufferData* UDrawCallBuilder::GetTransform() const
{
	return transform_;
}

UParametersBufferData* UDrawCallBuilder::GetParameters() const
{
	return parameters_;
}
