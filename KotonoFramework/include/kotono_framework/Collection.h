#pragma once
#include <functional>
#include <vector>
#include <unordered_set>
template<typename T>
class KtCollection
{
public:
	typedef std::function<bool(const T&)> Filter;

	struct Iterator
	{
		Iterator(const std::vector<T>& data, const std::vector<Filter>& filters, size_t pos)
			: data_(data), filters_(filters), pos_(pos)
		{
			AdvanceToNextValid();
		}

		const T& operator*() const
		{
			return data_[pos_];
		}

		Iterator& operator++()
		{
			++pos_;
			AdvanceToNextValid();
			return *this;
		}

		bool operator!=(const Iterator& other) const
		{
			return pos_ != other.pos_;
		}

	private:
		void AdvanceToNextValid()
		{
			while (pos_ < data_.size() && !PassesFilters(data_[pos_]))
			{
				++pos_;
			}
		}

		bool PassesFilters(const T& item) const
		{
			for (const Filter& filter : filters_)
			{
				if (!filter(item))
				{
					return false;
				}
			}
			return true;
		}

		const std::vector<T>& data_;
		const std::vector<Filter>& filters_;
		size_t pos_;
	};

	explicit KtCollection(const std::vector<T>& data) 
		: data_(data) {}

	explicit KtCollection(std::vector<T>&& data) 
		: data_(std::move(data)) {}

	explicit KtCollection(const std::unordered_set<T>& data)
		: data_(data.begin(), data.end()) {}

	void AddFilter(Filter&& filter)
	{
		filters_.push_back(std::move(filter));
	}

	void AddFilter(const Filter& filter)
	{
		filters_.push_back(filter);
	}

	const std::vector<T> GetVector() const
	{
		std::vector<T> result;
		result.reserve(data_.size());

		for (const T& item : *this)
		{
			result.push_back(item);
		}

		return result;
	}

	const std::unordered_set<T> GetUnorderedSet() const
	{
		std::unordered_set<T> result;
		result.reserve(data_.size());

		for (const T& item : *this)
		{
			result.insert(item);
		}

		return result;
	}

	T GetFirst()
	{
		for (const T& item : *this)
		{
			return item;
		}
	}

	Iterator begin() const { return Iterator(data_, filters_, 0); }
	Iterator end() const { return Iterator(data_, filters_, data_.size()); }

private:
	std::vector<T> data_;
	std::vector<Filter> filters_;
};

