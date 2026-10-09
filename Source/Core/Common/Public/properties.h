#pragma once

#include "macro_utils.h"
#include "Logging/log.h"
#include "type_traits.h"

template <typename T>
using getter_return_t = optimal_t<T>;

template <typename T>
using setter_param_t = optimal_t<T>;

#define PROP_ACCESS_(Type)					getter_return_t<Type>
#define PROP_ACCESS_Value(Type)				Type
#define PROP_ACCESS_Reference(Type)			Type&
#define PROP_ACCESS_ConstReference(Type)	Type const&

#define FUNC_ACCESS_				const
#define FUNC_ACCESS_Value			const
#define FUNC_ACCESS_Reference
#define FUNC_ACCESS_ConstReference	const

#define GET_PROP_ACCESS(...) MACRO_CONCAT(PROP_ACCESS_, __VA_ARGS__)
#define GET_FUNC_ACCESS(...) MACRO_CONCAT(FUNC_ACCESS_, __VA_ARGS__)

#define Getter(Type, Variable, GetterName, ...) \
	auto Get##GetterName() GET_FUNC_ACCESS(__VA_ARGS__) -> GET_PROP_ACCESS(__VA_ARGS__)(Type) { return Variable; }

#define Setter(Type, Variable, SetterName) \
	void Set##SetterName(setter_param_t<Type> value) { Variable = value; }

#define GetterAndSetter(GetterType, SetterType, Variable, PropertyName, ...)	\
	Getter(GetterType, Variable, PropertyName, __VA_ARGS__)						\
	Setter(SetterType, Variable, PropertyName)
	
#define ReadonlyProperty(Type, Variable, PropertyName, ...) private:	\
	Type Variable;														\
public:																	\
	Getter(Type, Variable, PropertyName, __VA_ARGS__)					\
private:

#define WritableProperty(Type, Variable, PropertyName, ...) private:	\
	Type Variable;														\
public:																	\
	Getter(Type, Variable, PropertyName, __VA_ARGS__)					\
	Setter(Type, Variable, PropertyName)								\
private:
