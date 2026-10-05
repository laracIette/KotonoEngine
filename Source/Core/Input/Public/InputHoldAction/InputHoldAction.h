#pragma once
#include <types.h>
class UInputHoldAction final
{
public:
	b8 Update(const f32 delta);

	void Reset();

	f32 GetActuationTime() const;
	f32 GetRepeatTime() const;

	void SetActuationTime(f32 actuationTime);
	void SetRepeatTime(f32 repeatTime);

private:
	b8 wasPlaying_;
	b8 isRepeating_;
	f32 currentTime_;
	f32 actuationTime_;
	f32 repeatTime_;
};