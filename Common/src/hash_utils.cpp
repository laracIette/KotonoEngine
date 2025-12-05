#include "hash_utils.h"

void combine(size_t& h, const size_t v) noexcept
{
    h ^= v + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
}
