#pragma once
#include <kotono_framework/Pool.h>
#include <type_traits>
template <class T>
class UPtr;

class UPtrOwnerBase
{

public:
	virtual ~UPtrOwnerBase() = default;

	virtual size_t GetIndex() const = 0;
	virtual void SetIndex(const size_t index) = 0;

	virtual void Set(void* pointer) = 0;
	virtual void* Get() const = 0;
};

template <class T>
class UPtrOwner : public UPtrOwnerBase
{
	using Child = UPtr<T>;
	friend Child;
	friend class SObjectManager;

public:
	UPtrOwner() : pointer_(nullptr) {}
	
	~UPtrOwner()
	{
		for (Child* child : children_)
		{
			child->owner_ = nullptr;
		}
	}

	void Set(void* pointer) override
	{
		pointer_ = static_cast<T*>(pointer);
	}

	size_t GetIndex() const override
	{
		return index_;
	}

	void SetIndex(const size_t index) override
	{
		index_ = index;
	}

	void* Get() const override
	{
		return pointer_;
	}

private:
	T* pointer_{ nullptr };
	size_t index_{ 0 };
	KtPool<Child*> children_{};
};

template <class T>
class UPtr
{
	using Owner = UPtrOwner<T>;
	friend Owner;

public:
	UPtr() {}

	UPtr(Owner* owner)
	{
		SetOwner(owner);
	}

	template <class U>
		requires std::is_base_of_v<T, U>
	UPtr(const UPtr<U>& other)
	{
		SetOwner(reinterpret_cast<Owner*>(other.GetOwner()));
	}

	~UPtr()
	{
		SetOwner(nullptr);
	}

	constexpr Owner* GetOwner() const noexcept
	{
		return owner_;
	}

	UPtr& operator=(const UPtr& other)
	{
		if (this == &other)
		{
			return *this;
		}
		SetOwner(other.owner_);
		return *this;
	}

	constexpr T* Get() const noexcept
	{
		return owner_->pointer_;
	}

	constexpr T* operator->() const noexcept
	{
		return owner_->pointer_;
	}

	constexpr operator bool() const noexcept
	{
		return owner_ && owner_->pointer_;
	}

	constexpr bool operator==(T* other) const noexcept
	{
		return owner_ && owner_->pointer_ == other;
	}

private:
	Owner* owner_{ nullptr };
	size_t index_{ 0 };

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

