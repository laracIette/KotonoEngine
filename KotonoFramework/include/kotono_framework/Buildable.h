#pragma once
#include <functional>
#include <variant>

template <typename T>
class KtBuildable final
{
    using ValueFunction = std::function<T()>;

public:
    KtBuildable() : storage_(T{}) {}
    KtBuildable(const T& value) : storage_(value) {}
    KtBuildable(ValueFunction&& valueFunction) : storage_(std::move(valueFunction)) {}

    operator T() const
    {
        if (std::holds_alternative<T>(storage_))
        {
            return std::get<T>(storage_);
        }
        else
        {
            return std::get<ValueFunction>(storage_)();
        }
    }

private:
    std::variant<T, ValueFunction> storage_;
};