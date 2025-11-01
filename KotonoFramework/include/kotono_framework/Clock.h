#pragma once
class KtClock final
{
	friend class KtFramework;

private:
	void Init();

public:
	// Current exact UTC Time since Epoch in seconds.
	double ExactUTC() const;
	// Current Time since the start of the program in seconds.
	float Now() const;

private:
	double _startUTC;
};