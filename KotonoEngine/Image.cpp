#include "Image.h"
#include <iostream>

Image::Image() :
	_texture(nullptr)
{
}

const bool Image::GetIsDraw() const
{
	return _texture && Base::GetIsDraw();
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

void Image::UpdateShader() const
{
	Base::UpdateShader();

}

void Image::Draw() const
{
	_texture->Draw();
}
