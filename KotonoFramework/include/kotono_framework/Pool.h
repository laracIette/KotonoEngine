#pragma once
#include <vector>
template <typename ValueType>
class KtPool final
{
private:
	using IteratorType = std::vector<ValueType>::iterator;
	using ConstIteratorType = std::vector<ValueType>::const_iterator;
	using ConditionFunction = std::function<bool(const ValueType&)>;

public:
	void Add(const ValueType& value)
	{
		data_.push_back(value);
	}

	void Add(ValueType&& value)
	{
		data_.push_back(std::move(value));
	}

	void Remove(const ValueType& value)
	{
		const auto it = std::find(data_.begin(), data_.end(), value);
		if (it == data_.end())
		{
			return;
		}

		const size_t index = std::distance(data_.begin(), it);
		RemoveAt(index);
	}

	void RemoveAt(const size_t index)
	{
		if (index >= data_.size())
		{
			return;
		}

		if (index != data_.size() - 1)
		{
			// Only swap if not last
			data_[index] = std::move(data_.back());  
		}
		data_.pop_back();
	}

	void RemoveIf(const ConditionFunction& condition)
	{
		// Have to check backwards to avoid skipping condition checks
		for (int64_t i = static_cast<int64_t>(data_.size()) - 1; i >= 0; i--)
		{
			if (condition(data_[i]))
			{
				RemoveAt(i);
			}
		}
	}

	constexpr void Clear() noexcept
	{
		data_.clear();
	}

	constexpr void Append(const KtPool& pool) noexcept
	{
		data_.insert(end(), pool.begin(), pool.end());
	}

	constexpr void Merge(KtPool& pool) noexcept
	{
		data_.insert(end(), std::make_move_iterator(pool.begin()), std::make_move_iterator(pool.end()));
		pool.Clear();
	}

	constexpr IteratorType begin() noexcept
	{
		return data_.begin(); 
	}

	constexpr IteratorType end() noexcept
	{ 
		return data_.end(); 
	}

	constexpr ConstIteratorType begin() const noexcept
	{
		return data_.begin();
	}

	constexpr ConstIteratorType end() const noexcept
	{ 
		return data_.end(); 
	}

	constexpr const size_t Size() const noexcept
	{
		return data_.size();
	}

	// Returns size_t max if size == 0
	constexpr const size_t LastIndex() const noexcept
	{
		return data_.size() - 1;
	}

	constexpr const bool Empty() const noexcept
	{
		return data_.empty();
	}

	const ValueType& operator[](const size_t index) const noexcept
	{
		return data_[index];
	}

	ValueType& operator[](const size_t index) noexcept
	{
		return data_[index];
	}

private:
	std::vector<ValueType> data_;
};

