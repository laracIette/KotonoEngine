#pragma once
#include "Path.h"
class SPathManager final
{
public:
	static UPath const& Engine();
	static UPath const& Project();

private:
	static UPath projectPath_;
};
