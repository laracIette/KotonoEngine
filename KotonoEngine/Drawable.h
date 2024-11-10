#pragma once
#include "Object.h"
#include "Material.h"
class Drawable :
    public Object
{
    BASECLASS(Object)

public:
    Drawable();
    ~Drawable();
    
    virtual const bool GetIsDraw() const;
    void SetIsDraw(const bool isDraw);
    
    Material* GetMaterial() const;
    virtual void SetMaterial(Material* material);

    virtual void UpdateShader() const;
    virtual void Draw() const;

private:
    bool _isDraw;

    Material* _material;
};

