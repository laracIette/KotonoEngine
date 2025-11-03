#pragma once
#include "Object.h"
#include <kotono_framework/Event.h>
#include "Duration.h"
class KTask : public KObject
{
	BASECLASS(KObject)

protected:
	void Init() override;
	void Update() override;

public:
	const UDuration& GetDuration() const;
	KtEvent<>& EventUpdate();
	KtEvent<>& EventCompleted();

	void SetDuration(const UDuration& duration);

	void Start();
	void Stop();

private:
	bool isPlaying_;
	UDuration current_;
	UDuration duration_;
	KtEvent<> eventUpdate_;
	KtEvent<> eventCompleted_;
};

