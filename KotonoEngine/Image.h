#pragma once
#include "Drawable2D.h"
#include "ImageTexture.h"
class Image final :
    public Drawable2D
{
    BASECLASS(Drawable2D)

public:
    Image();

    const bool GetIsDraw() const override;

    ImageTexture* GetTexture() const;

    void SetMaterial(Material* material) override;
    void SetTexture(ImageTexture* texture);

    void UpdateShader() const override;
    void Draw() const override;

private:
    ImageTexture* _texture;

    void UpdateTextureShader();
};

