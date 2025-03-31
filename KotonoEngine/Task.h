#pragma once
#include "Object.h"
#include <kotono_framework/Event.h>
class OTask : public OObject
{
	BASECLASS(OObject)

public:
	void Update() override;

	const float GetDuration() const;
	KtEvent& GetEventUpdate();

	void SetDuration(const float duration);

	void Start();
	void Stop();

private:
	bool _isPlaying;
	float _startTime;
	float _duration;
	KtEvent _updateEvent;
};

