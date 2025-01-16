#pragma once
#include "Drawable2D.h"
class RoundedBox :
    public Drawable2D
{
    BASECLASS(Drawable2D)

public:
    void UpdateShader() const override;
    void Draw() const override;

    float GetFallOff() const;
    float GetCornerSize() const;

    float GetTargetFallOff() const;
    float GetTargetCornerSize() const;

    void SetTargetFallOff(float targetFallOff);
    void SetTargetCornerSize(float targetCornerSize);

    virtual void SetRelativeSize(const glm::vec2& relativeSize);
    virtual void SetWorldSize(const glm::vec2& worldSize);

protected:
    float _targetFallOff;
    float _targetCornerSize;

    float _fallOff;
    float _cornerSize;

    virtual void UpdateValues();

    const glm::mat4 GetModelMatrix() const override;

    const glm::vec4 GetSides() const;
};

