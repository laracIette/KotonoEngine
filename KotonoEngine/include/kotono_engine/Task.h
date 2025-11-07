#pragma once
#include "generated/Task.generated.h"
#include "Object.h"
#include <kotono_framework/Event.h>
#include "Duration.h"
class KTask : public KObject
{
	GENERATED_KTASK()

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

