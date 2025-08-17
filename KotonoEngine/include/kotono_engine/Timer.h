#pragma once
#include "Object.h"
#include <kotono_framework/Event.h>
class KTimer : public KObject
{
	BASECLASS(KObject)

protected:
	void Update() override;

public:
	bool GetIsPlaying() const;
	bool GetIsRepeat() const;
	const UDuration& GetDuration() const;
	KtEvent<>& GetEventCompleted();

	void SetIsRepeat(const bool isRepeat);
	void SetDuration(const UDuration& duration);

	void Start(const bool isOverride = true);
	void Stop();

private:
	bool isPlaying_;
	bool isRepeat_;
	UDuration start_;
	UDuration targetDuration_;
	UDuration currentDuration_;
	KtEvent<> eventCompleted_;

	UDuration GetNow() const;
};

