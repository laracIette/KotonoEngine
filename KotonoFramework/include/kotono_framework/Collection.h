#pragma once
#include <functional>
#include <vector>
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
			for (const auto& filter : filters_)
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

	void AddFilter(Filter&& filter)
	{
		filters_.push_back(std::move(filter));
	}

	void AddFilter(const Filter& filter)
	{
		filters_.push_back(filter);
	}

	Iterator begin() const { return Iterator(data_, filters_, 0); }
	Iterator end() const { return Iterator(data_, filters_, data_.size()); }

private:
	std::vector<T> data_;
	std::vector<Filter> filters_;
};

