#pragma once
#include "Drawable2D.h"
#include "ImageTexture.h"
class Image final :
    public Drawable2D
{
    BASECLASS(Drawable2D)

public:
    Image();

    ImageTexture* GetTexture() const;

    void SetMaterial(Material* material) override;
    void SetTexture(ImageTexture* texture);

    void UpdateTextureShader();

    void Draw() const override;

private:
    ImageTexture* _texture;
};

