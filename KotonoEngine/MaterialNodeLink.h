#pragma once
#include "MaterialNode.h"
#include "MaterialNodeInput.h"
class MaterialNodeLink
{
public:
	MaterialNodeLink(MaterialNode* start, MaterialNodeInput* end);

	MaterialNode* GetStart() const;
	MaterialNodeInput* GetEnd() const;

private:
	MaterialNode* _start;
	MaterialNodeInput* _end;
};

