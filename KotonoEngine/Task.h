#pragma once
#include "Object.h"
#include <kotono_framework/Event.h>
class KTask : public KObject
{
	BASECLASS(KObject)

public:
	void Update() override;

	const float GetDuration() const;
	KtEvent<>& GetEventUpdate();
	KtEvent<>& GetEventCompleted();

	void SetDuration(const float duration);

	void Start();
	void Stop();

private:
	bool isPlaying_;
	float startTime_;
	float duration_;
	KtEvent<> eventUpdate_;
	KtEvent<> eventCompleted_;
};

