#include "Object/ObjectReflectionBase.h"

KObjectReflectionBase::KObjectReflectionBase()
	: Pointer{ this }
{
}

KObjectReflectionBase::~KObjectReflectionBase()
{
	Pointer.Invalidate();
}

void KObjectReflectionBase::SerializeTo(nlohmann::json& json) const
{
}

void KObjectReflectionBase::DeserializeFrom(nlohmann::json const& json)
{
}

void KObjectReflectionBase::PopulateMemberVariables(UList<UVariableInfo>& variables) const
{
}

auto KObjectReflectionBase::GetMemberVariables() const -> UList<UVariableInfo>
{
	UList<UVariableInfo> variables{};
	PopulateMemberVariables(variables);
	return variables;
}
