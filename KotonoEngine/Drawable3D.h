#pragma once
#include "Drawable.h"
#include "Transform.h"
class Drawable3D :
    public Drawable
{
    BASECLASS(Drawable)

public:
    Drawable3D();
    ~Drawable3D();

    const bool GetIsDraw() const override;

    Transform* GetTransform() const;

    Drawable3D* GetParent() const;
    void SetParent(Drawable3D* parent);

private:
    Transform* _transform;
    Drawable3D* _parent;
};

