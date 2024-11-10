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

	const bool GetIsDraw() const override;

	Rect* GetRect() const;
	
	Drawable2D* GetParent() const;
	void SetParent(Drawable2D* parent);

	virtual void UpdateShader() const override;
	virtual void Draw() const override;

private:
	Rect* _rect;
	Drawable2D* _parent;
};

