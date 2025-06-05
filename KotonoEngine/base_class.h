#pragma once
#define BASECLASS(type) \
private: \
	typedef type Base; \
	using Base::Base;