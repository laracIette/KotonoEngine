#pragma once
#include <kotono_common/types.h>
class SClock final
{
public:
	// Exact UTC time since Epoch in seconds.
	static f64 ExactUTC();
	// UTC time at which the program started.
	static f64 StartUTC();
	// Elapsed time since the start of the program in seconds.
	static f32 Now();

private:
	static f64 startUTC_;
};