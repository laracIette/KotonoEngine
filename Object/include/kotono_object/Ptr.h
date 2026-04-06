#pragma once
#include <kotono_common/Pool.h>
#include <type_traits>
#include <concepts>

class UPtrBase
{
public:
	virtual ~UPtrBase() = default;
	virtual void Invalidate() noexcept = 0;
};

class UPtrOwner final
{
	template <typename T>
	friend class UPtr;

public:
	UPtrOwner() : pointer_(nullptr), children_() {}

	~UPtrOwner()
	{
		for (UPtrBase* child : children_)
		{
			child->Invalidate();
		}
	}

	constexpr void Set(void* pointer) noexcept
	{
		pointer_ = pointer;
	}

	constexpr void* Get() const noexcept
	{
		return pointer_;
	}

private:
	void* pointer_;
	KtPool<UPtrBase*> children_;
};

template <class T>
class UPtr final : public UPtrBase
{
private:
	template <typename U> 
	friend class UPtr;

	using Owner = UPtrOwner;
	friend class Owner;

	friend std::hash<UPtr>;

	using RemoveConstT = std::remove_const_t<T>;

public:
	using PointerType = T;

public:
	UPtr() : owner_(nullptr), index_(0) {}

	UPtr(Owner* owner) : UPtr()
	{
		SetOwner(owner);
	}
	
	UPtr(const UPtr& other) : UPtr()
	{
		SetOwner(other.owner_);
	}

	template <typename From>
		requires std::is_convertible_v<From*, T*>
	UPtr(const UPtr<From>& other) : UPtr()
	{
		SetOwner(other.owner_);
	}

	~UPtr() override
	{
		SetOwner(nullptr);
	}

	void Invalidate() noexcept override
	{
		owner_ = nullptr;
	}

	template <typename From>
		requires std::is_convertible_v<From*, T*>
	UPtr& operator=(const UPtr<From>& other)
	{
		SetOwner(other.owner_);
		return *this;
	}

	UPtr& operator=(const UPtr& other)
	{
		if (this != &other)
		{
			SetOwner(other.owner_);
		}
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
		return owner_ == other.owner_;
	}

	constexpr bool operator==(PointerType* ptr) const noexcept
	{
		return (!owner_ && !ptr) || (owner_ && owner_->Get() == ptr);
	}

	constexpr PointerType* Get() const noexcept
	{
		return static_cast<PointerType*>(owner_->Get());
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
		return owner_ && owner_->Get();
	}

	constexpr Owner* GetOwner() const noexcept
	{
		return owner_;
	}

private:
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
				static_cast<UPtr*>(owner_->children_[index_])->index_ = index_;
			}
		}

		owner_ = owner;
		
		if (owner_)
		{
			owner_->children_.Add(this);
			index_ = owner_->children_.LastIndex();
		}
	}

private:
	Owner* owner_;
	size index_;
};

template <typename Derived, typename Base>
	requires std::is_base_of_v<Base, Derived>
inline UPtr<Derived> TryCast(const UPtr<Base>& ptr)
{
	if (ptr && dynamic_cast<Derived*>(ptr.Get()))
	{
		return UPtr<Derived>(ptr.GetOwner());
	}
	return nullptr;
}

template <typename T>
struct std::hash<UPtr<T>>
{
	::size operator()(const UPtr<T>& ptr) const noexcept
	{
		return std::hash<void*>{}(ptr.owner_);
	}
};
