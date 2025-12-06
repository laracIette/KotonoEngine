#pragma once
class KtClock final
{
public:
	// Current exact UTC Time since Epoch in seconds.
	static double ExactUTC();
	// Current Time since the start of the program in seconds.
	static float Now();

private:
	static double startUTC_;
};