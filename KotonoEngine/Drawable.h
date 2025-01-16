#pragma once
#include "Updatable.h"
#include "Shader.h"
#include "Color.h"
class Drawable :
    public Updatable
{
    BASECLASS(Updatable)

public:
    Drawable();
    
    virtual const bool GetIsDraw() const;
    void SetIsDraw(bool isDraw);
    
    Shader* GetShader() const;
    virtual void SetShader(Shader* shader);

    const Color& GetColor() const;
    void SetColor(const Color& color);

    virtual void UpdateShader() const;
    virtual void Draw() const;

protected:
    virtual const glm::mat4 GetModelMatrix() const = 0;

private:
    bool _isDraw;

    Shader* _shader;

    Color _color;
};

