#pragma once
#include <kotono_common/Pool.h>
#include <type_traits>
#include <concepts>

template <typename T, typename Derived>
concept BaseOf = std::is_base_of_v<T, Derived>;

template <typename T, typename Base>
concept DerivedFrom = std::is_base_of_v<Base, T>;

template <class T>
class UPtr;

class UPtrOwnerBase
{
public:
	virtual ~UPtrOwnerBase() = default;

	virtual void Set(void* pointer) = 0;
	virtual void* Get() const = 0;
};

template <class T>
class UPtrOwner final : public UPtrOwnerBase
{
	using Child = UPtr<T>;
	friend Child;
	friend UPtr<const T>;

public:
	using PointerType = T;

public:
	UPtrOwner() = default;
	
	~UPtrOwner()
	{
		for (Child* child : children_)
		{
			child->owner_ = nullptr;
		}
	}

	void Set(void* pointer) override
	{
		pointer_ = static_cast<PointerType*>(pointer);
	}

	void* Get() const override
	{
		return pointer_;
	}

private:
	PointerType* pointer_{ nullptr };
	KtPool<Child*> children_{};
};

template <class T>
class UPtr final
{
private:
	template <typename U> 
	friend class UPtr;

	using Owner = UPtrOwner<std::remove_const_t<T>>;
	friend Owner;

public:
	using PointerType = T;

public:
	UPtr() = default;

	UPtr(Owner* owner)
	{
		SetOwner(owner);
	}
	
	UPtr(const UPtr& other)
	{
		SetOwner(other.owner_);
	}

	template <typename From>
		requires std::is_convertible_v<From*, T*>
	UPtr(const UPtr<From>& other)
	{
		SetOwner(reinterpret_cast<Owner*>(other.owner_));
	}

	~UPtr()
	{
		SetOwner(nullptr);
	}

	template <typename From>
		requires std::is_convertible_v<From*, T*>
	UPtr& operator=(const UPtr<From>& other)
	{
		SetOwner(reinterpret_cast<Owner*>(other.owner_));
		return *this;
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

	constexpr bool operator==(const UPtr& other) const noexcept
	{
		return owner_ == other.owner_;
	}

	template <typename From>
		requires std::is_convertible_v<From*, T*>
	constexpr bool operator==(const UPtr<From>& other) const noexcept
	{
		return owner_ == reinterpret_cast<Owner*>(other.owner_);
	}

	constexpr bool operator==(PointerType* ptr) const noexcept
	{
		return (!owner_ && !ptr) || (owner_ && owner_->pointer_ == ptr);
	}

	constexpr PointerType* Get() const noexcept
	{
		return owner_->pointer_;
	}

	constexpr PointerType* operator->() const noexcept
	{
		return Get();
	}

	constexpr PointerType& operator*() const noexcept
	{
		return *Get();
	}

	constexpr operator bool() const noexcept
	{
		return owner_ && owner_->pointer_;
	}

	constexpr Owner* GetOwner() const noexcept
	{
		return owner_;
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
			owner_->children_.Add(reinterpret_cast<UPtr<std::remove_const_t<T>>*>(this));
			index_ = owner_->children_.LastIndex();
		}
	}
};

template <typename Derived, typename Base>
	requires std::is_base_of_v<Base, Derived>
inline UPtr<Derived> TryCast(const UPtr<Base>& ptr)
{
	if (ptr && dynamic_cast<Derived*>(ptr.Get()))
	{
		return reinterpret_cast<UPtrOwner<std::remove_const_t<Derived>>*>(ptr.GetOwner());
	}
	return nullptr;
}
