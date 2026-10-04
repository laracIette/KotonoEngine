#pragma once
#include <kotono_common/types.h>
class UInputHoldAction final
{
public:
	bool Update(const f32 delta);

	void Reset();

	f32 GetActuationTime() const;
	f32 GetRepeatTime() const;

	void SetActuationTime(const f32 actuationTime);
	void SetRepeatTime(const f32 repeatTime);

private:
	bool wasPlaying_;
	bool isRepeating_;
	f32 currentTime_;
	f32 actuationTime_;
	f32 repeatTime_;
};