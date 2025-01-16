#pragma once
#include "Drawable.h"
#include "Rect.h"
class Drawable2D :
	public Drawable
{
	BASECLASS(Drawable)

public:
	Drawable2D();
	~Drawable2D();

	void Update(float deltaTime) override;

	const bool GetIsDraw() const override;

	void Draw() const override;

	Rect* GetRect() const;
	
	Drawable2D* GetParent() const;
	void SetParent(Drawable2D* parent);

	const int GetLayer() const;
	void SetLayer(const int layer);

protected:
	const glm::mat4 GetModelMatrix() const override;

private:
	Rect* _rect;

	Drawable2D* _parent;

	int _layer;
};