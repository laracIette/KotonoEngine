#pragma once
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
struct VertexArraySetup final
{
	VertexArrayObject VertexArrayObject;
	VertexBufferObject VertexBufferObject;
	ElementBufferObject ElementBufferObject;
};

