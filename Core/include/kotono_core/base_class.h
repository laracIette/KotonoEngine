#pragma once
#define BASECLASS(type) \
private: \
	using Base = type; \
	using Base::Base;