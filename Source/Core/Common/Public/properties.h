#pragma once

#include "macro_utils.h"
#include "Logging/log.h"
#include "type_traits.h"

#define PROP_ACCESS_(Type)			TOptimal<Type>
#define PROP_ACCESS_Reference(Type)	Type&

#define FUNC_ACCESS_		        const
#define FUNC_ACCESS_Reference

#define GET_PROP_ACCESS(...) MACRO_CONCAT(PROP_ACCESS_, __VA_ARGS__)
#define GET_FUNC_ACCESS(...) MACRO_CONCAT(FUNC_ACCESS_, __VA_ARGS__)

#define Getter(Type, Variable, GetterName, ...) \
	auto Get##GetterName() GET_FUNC_ACCESS(__VA_ARGS__) -> GET_PROP_ACCESS(__VA_ARGS__)(Type) { return Variable; }

#define Setter(Type, Variable, SetterName) \
	void Set##SetterName(TOptimal<Type> value) { Variable = value; }

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

/// A protected member variable with a public getter
#define ProtectedProperty(Type, Property, ...) protected:	\
	Type Property;											\
public:														\
	Getter(Type, Property, Property, __VA_ARGS__)			\
private:
