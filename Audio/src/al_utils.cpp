#include "al_utils.h"

#include <AL/al.h>
#include <format>
#include <stdexcept>

void ALCheckAndThrow(int result, char const* message)
{
	if (result != AL_NO_ERROR)
	{
		throw std::runtime_error{ std::format("{0} (ALenum: {1})", message, result) };
	}
}
