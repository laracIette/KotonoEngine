#pragma once
#include "generated/Timer.generated.h"
#include "Object.h"
#include <kotono_framework/Event.h>
class KTimer : public KObject
{
	GENERATED_KTIMER()

protected:
	void Init() override;
	void Update() override;

public:
	bool GetIsPlaying() const;
	bool GetIsRepeat() const;
	const UDuration& GetDuration() const;
	KtEvent<>& EventCompleted();

	void SetIsRepeat(const bool isRepeat);
	void SetDuration(const UDuration& duration);

	void Start(const bool isOverride = true);
	void Stop();

private:
	bool isPlaying_;
	bool isRepeat_;
	UDuration duration_;
	UDuration current_;
	KtEvent<> eventCompleted_;
};

