#pragma once
#include <kotono_framework/Pool.h>
template <class T>
class UPtr;

class UPtrOwnerBase
{
public:
	virtual ~UPtrOwnerBase() = default;
};

template <class T>
class UPtrOwner : public UPtrOwnerBase
{
	using Child = UPtr<T>;
	friend Child;
	friend class SObjectManager;

public:
	UPtrOwner(T* ptr) : ptr_(ptr) {}

private:
	T* ptr_;
	size_t index_;
	KtPool<Child*> children_;
};

template <class T>
class UPtr
{
	using Owner = UPtrOwner<T>;
	friend Owner;

public:
	UPtr(Owner* owner = nullptr)
	{
		SetOwner(owner);
	}

	~UPtr()
	{
		SetOwner(nullptr);
	}

	UPtr& operator=(UPtr& other)
	{
		SetOwner(other.owner_);
	}

	T* operator->() const noexcept
	{
		return owner_->ptr_;
	}

private:
	Owner* owner_;
	size_t index_;

	void SetOwner(Owner* owner)
	{
		if (owner == owner_)
		{
			return;
		}

		if (owner_)
		{
			if (owner_->children_.RemoveAt(index_) == KtPoolRemoveResult::ItemSwappedAndRemoved)
			{
				owner_->children_[index_]->index_ = index_;
			}
		}

		owner_ = owner;
		
		if (owner_)
		{
			owner_->children_.Add(this);
			index_ = owner_->children_.LastIndex();
		}
	}
};

