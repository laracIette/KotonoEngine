#include "macro_utils.h"
#include "types.h"
#include "type_traits.h"

/// Bits to bytes
#define BYTES(Bits) (MACRO_CONCAT(Bits, llu) / 8llu)

static_assert(sizeof(i8)  == BYTES(8));
static_assert(sizeof(i16) == BYTES(16));
static_assert(sizeof(i32) == BYTES(32));
static_assert(sizeof(i64) == BYTES(64));

static_assert(sizeof(u8)  == BYTES(8));
static_assert(sizeof(u16) == BYTES(16));
static_assert(sizeof(u32) == BYTES(32));
static_assert(sizeof(u64) == BYTES(64));

static_assert(sizeof(f32) == BYTES(32));
static_assert(sizeof(f64) == BYTES(64));

static_assert(sizeof(b8)  == BYTES(8));

namespace
{
	struct CheapCopyable{ i64 a; };
	struct ExpensiveCopyable{ i64 a, b; };
	struct NotCopyable{ ~NotCopyable() {} };
	
	struct Base{};
	struct Derived : Base{};
}

static_assert(VIsCopyableOrCheap<CheapCopyable>);
static_assert(!VIsCopyableOrCheap<ExpensiveCopyable>);
static_assert(!VIsCopyableOrCheap<NotCopyable>);

static_assert(std::is_same_v<TOptimal<CheapCopyable>, CheapCopyable>);
static_assert(std::is_same_v<TOptimal<ExpensiveCopyable>, ExpensiveCopyable const&>);
static_assert(std::is_same_v<TOptimal<NotCopyable>, NotCopyable const&>);
	
static_assert(VIsStrictBaseOf<Base, Derived>);
static_assert(!VIsStrictBaseOf<Base, Base>);
static_assert(!VIsStrictBaseOf<Derived, Base>);
