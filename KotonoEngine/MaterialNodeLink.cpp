#include "MaterialNodeLink.h"

MaterialNodeLink::MaterialNodeLink(MaterialNode* start, MaterialNodeInput* end) :
	_start(start), _end(end)
{
}

MaterialNode* MaterialNodeLink::GetStart() const
{
	return _start;
}

MaterialNodeInput* MaterialNodeLink::GetEnd() const
{
	return _end;
}
