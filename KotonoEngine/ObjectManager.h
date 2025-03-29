#pragma once
#include "Mesh.h"
#include "Image.h"
class KObjectManager
{
public:
	void Init();
	void Update();
	void Cleanup();

private:
	void Quit();

	TMesh _mesh1;
	TMesh _mesh2;
	RImage _image1;
};

