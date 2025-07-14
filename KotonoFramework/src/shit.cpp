#include "Event.h"

static int64_t EventInstances{ 0 };

const int64_t incrementEventInstances()
{
	return ++EventInstances;
}
const int64_t decrementEventInstances()
{
	return --EventInstances;
}