#pragma once
#include <cstddef>
#include <stdint.h>

using i8  =		int8_t;
using i16 =		int16_t;
using i32 =		int32_t;
using i64 =		int64_t;

using u8  =		uint8_t;
using u16 =		uint16_t;
using u32 =		uint32_t;
using u64 =		uint64_t;

using size =	size_t;

using f32 =		float;
using f64 =		double;

using b8 =		bool;

static_assert(sizeof(i8)  ==  8llu / 8llu);
static_assert(sizeof(i16) == 16llu / 8llu);
static_assert(sizeof(i32) == 32llu / 8llu);
static_assert(sizeof(i64) == 64llu / 8llu);

static_assert(sizeof(u8)  ==  8llu / 8llu);
static_assert(sizeof(u16) == 16llu / 8llu);
static_assert(sizeof(u32) == 32llu / 8llu);
static_assert(sizeof(u64) == 64llu / 8llu);

static_assert(sizeof(f32) == 32llu / 8llu);
static_assert(sizeof(f64) == 64llu / 8llu);

static_assert(sizeof(b8)  ==  8llu / 8llu);

