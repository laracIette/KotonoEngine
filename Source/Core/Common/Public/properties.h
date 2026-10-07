#pragma once

#include "macro_utils.h"
#include "Logging/log.h"
#include <stdexcept>

#define _PROP_ACCESS_				const&
#define _PROP_ACCESS_Value 
#define _PROP_ACCESS_Reference		&
#define _PROP_ACCESS_ConstReference const&

#define _FUNC_ACCESS_				const
#define _FUNC_ACCESS_Value			const
#define _FUNC_ACCESS_Reference 
#define _FUNC_ACCESS_ConstReference const

#define _GET_PROP_ACCESS(...) MACRO_CONCAT(_PROP_ACCESS_, __VA_ARGS__)
#define _GET_FUNC_ACCESS(...) MACRO_CONCAT(_FUNC_ACCESS_, __VA_ARGS__)

#define Getter(Type, Variable, GetterName, ...) \
	auto Get##GetterName() _GET_FUNC_ACCESS(__VA_ARGS__) -> Type _GET_PROP_ACCESS(__VA_ARGS__) { return Variable; }

#define Setter(Type, Variable, SetterName) \
	void Set##SetterName(Type const& value) { Variable = value; }

#define GetterAndSetter(Type, Variable, PropertyName, ...)	\
	Getter(Type, Variable, PropertyName, __VA_ARGS__)		\
	Setter(Type, Variable, PropertyName)
	
#define ReadonlyProperty(Type, Name, PropertyName, ...) private:	\
	Type Name;														\
public:																\
	Getter(Type, Name, PropertyName, __VA_ARGS__)					\
private:

#define WritableProperty(Type, Name, PropertyName, ...) private:	\
	Type Name;														\
public:																\
	GetterAndSetter(Type, Name, PropertyName, __VA_ARGS__)			\
private:
