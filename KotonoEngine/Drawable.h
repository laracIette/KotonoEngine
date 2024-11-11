#pragma once
#include "Object.h"
#include "Material.h"
class Drawable :
    public Object
{
    BASECLASS(Object)

public:
    Drawable();
    
    virtual const bool GetIsDraw() const;
    void SetIsDraw(bool isDraw);
    
    Material* GetMaterial() const;
    virtual void SetMaterial(Material* material);

    virtual void UpdateShader() const;
    virtual void Draw() const;

private:
    bool _isDraw;

    Material* _material;
};

