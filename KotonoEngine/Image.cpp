#include "Image.h"
#include <iostream>

Image::Image() :
	_texture(nullptr)
{
}

ImageTexture* Image::GetTexture() const
{
	return _texture;
}

void Image::SetMaterial(Material* material)
{
	Base::SetMaterial(material);
	UpdateTextureShader();
}

void Image::SetTexture(ImageTexture* texture)
{
	_texture = texture;
	UpdateTextureShader();
}

void Image::UpdateTextureShader()
{
	if (_texture && GetMaterial())
	{
		_texture->SetShader(GetMaterial()->GetShader());
	}
}

void Image::Draw() const
{
	if (_texture)
	{
		_texture->Draw();
	}
}
