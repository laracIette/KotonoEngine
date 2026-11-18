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

    constexpr USpan(USpan&& span) noexcept
        : pointer_{ span.pointer_ }
        , size_{ span.size_ }
    {}

    constexpr USpan(USpan const& span) noexcept
        : pointer_{ span.pointer_ }
        , size_{ span.size_ }
    {}

    constexpr USpan(ValueType* pointer, IndexType size) noexcept
        : pointer_{ pointer }
        , size_{ size }
    {}

    template <IndexType Size>
    constexpr USpan(ValueType(& array)[Size]) noexcept
        : pointer_{ array }
        , size_{ Size }
    {}

    template <typename Container>
    requires (!std::is_array_v<std::remove_reference_t<Container>> &&
          std::is_convertible_v<typename Container::value_type*, ValueType*>)
    constexpr USpan(Container const& container) noexcept
        : pointer_{ container.data() }
        , size_{ container.size() }
    {}

    constexpr auto begin() const noexcept -> ValueType* {
        return pointer_;
    }

    constexpr auto end() const noexcept -> ValueType* {
        return pointer_ + size_;
    }

    constexpr auto operator[](IndexType index) const noexcept -> ValueType& {
        return pointer_[index];
    }

    constexpr auto Data() const noexcept -> ValueType* {
        return pointer_;
    }

    constexpr auto Size() const noexcept -> IndexType {
        return size_;
    }

    constexpr auto SizeBytes() const noexcept -> IndexType {
        return size_ * sizeof(ValueType);
    }

    constexpr auto IsEmpty() const noexcept -> b8 {
        return size_ == 0;
    }

private:
    ValueType* pointer_;
    IndexType size_;
};

template <typename T>
struct IsCopyable<USpan<T>> final : std::true_type {};
