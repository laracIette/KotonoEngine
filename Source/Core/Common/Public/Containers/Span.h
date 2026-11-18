#pragma once
#include "type_traits.h"
#include "types.h"

template <typename T>
class USpan final
{
private:
    using ValueType = T;
    using IndexType = ::size;

public:
    using value_type = ValueType;

public:
    constexpr USpan() noexcept
        : pointer_{ nullptr }
        , size_{ 0 }
    {}

    constexpr USpan(ValueType* pointer, IndexType size) noexcept
        : pointer_{ pointer }
        , size_{ size }
    {}

    template <IndexType Size>
    constexpr USpan(ValueType (&array)[Size]) noexcept
        : pointer_{ array }
        , size_{ Size }
    {}

    template <typename Container>
    requires (!std::is_array_v<std::remove_reference_t<Container>> &&
          std::is_convertible_v<typename Container::value_type*, ValueType*>)
    constexpr USpan(Container& container) noexcept
        : pointer_{ container.data() }
        , size_{ container.size() }
    {}

    constexpr auto operator[](IndexType index) const noexcept -> ValueType& {
        return pointer_[index];
    }

    constexpr auto data() const noexcept -> ValueType* {
        return pointer_;
    }

    constexpr auto size() const noexcept -> IndexType {
        return size_;
    }

    constexpr auto empty() const noexcept -> b8 {
        return size_ == 0;
    }

    constexpr auto begin() const noexcept -> ValueType* {
        return pointer_;
    }

    constexpr auto end() const noexcept -> ValueType* {
        return pointer_ + size_;
    }

private:
    ValueType* pointer_;
    IndexType size_;
};

template <typename T>
struct IsCopyable<USpan<T>> final : std::true_type {};
