#include "InputHoldAction.h"

bool UInputHoldAction::Update(const f32 delta)
{
	currentTime_ += delta;

	if (isRepeating_ && currentTime_ > repeatTime_)
	{
		currentTime_ = 0.0f;
	}
	else if (currentTime_ > actuationTime_)
	{
		isRepeating_ = true;
		currentTime_ = 0.0f;
	}
	else if (wasPlaying_)
	{
		return false;
	}

	wasPlaying_ = true;

	return true;
}

void UInputHoldAction::Reset()
{
	wasPlaying_ = false;
	isRepeating_ = false;
	currentTime_ = 0.0f;
}

f32 UInputHoldAction::GetActuationTime() const
{
	return actuationTime_;
}

f32 UInputHoldAction::GetRepeatTime() const
{
	return repeatTime_;
}

void UInputHoldAction::SetActuationTime(const f32 actuationTime)
{
	actuationTime_ = actuationTime;
}

void UInputHoldAction::SetRepeatTime(const f32 repeatTime)
{
	repeatTime_ = repeatTime;
}