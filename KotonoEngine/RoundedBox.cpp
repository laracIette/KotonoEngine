#include "RoundedBox.h"

float RoundedBox::GetFallOff() const
{
	return _fallOff;
}

float RoundedBox::GetCornerSize() const
{
	return _cornerSize;
}

inline float RoundedBox::GetTargetFallOff() const
{
	return _targetFallOff;
}

inline void RoundedBox::SetTargetFallOff(float targetFallOff)
{
	_targetFallOff = targetFallOff;
	UpdateValues();
}

inline float RoundedBox::GetTargetCornerSize() const
{
	return _targetCornerSize;
}

inline void RoundedBox::SetTargetCornerSize(float targetCornerSize)
{
	_targetCornerSize = targetCornerSize;
	UpdateValues();
}

inline void RoundedBox::SetRelativeSize(const glm::vec2& relativeSize)
{
	GetRect()->SetRelativeSize(relativeSize);
	UpdateValues();
}

inline void RoundedBox::UpdateValues()
{
	glm::vec2 worldSize = GetRect()->GetWorldSize();

	float minSize = glm::min(worldSize.x, worldSize.y);

	// CornerSize has a minimum of 0, max of half the smallest dimension
	_cornerSize = glm::clamp(_targetCornerSize, 0.0f, minSize * 0.5f);

	// FallOff has a minimum of 0
	_fallOff = glm::max(0.0f, _targetFallOff);
}

inline glm::vec4 RoundedBox::GetSides() const
{
	/*glm::vec2 position(
		Window::Instance().GetViewport().Position.x + GetWorldPosition().x,
		Window::Instance().GetSize().y - Window::Instance().GetViewport().Position.y - GetWorldPosition().y
	);

	glm::vec2 worldSize = GetRect()->GetWorldSize();

	return glm::vec4(
		position.x - 0.5f * worldSize.x,
		position.x + 0.5f * worldSize.x,
		position.y + 0.5f * worldSize.y,
		position.y - 0.5f * worldSize.y
	);*/
}

inline void RoundedBox::UpdateShader() const
{
	/*RoundedBoxShader* roundedBoxShader = dynamic_cast<RoundedBoxShader*>(GetShader());
	if (roundedBoxShader)
	{
		roundedBoxShader->SetModel(GetModel());
		roundedBoxShader->SetColor(GetColor());
		roundedBoxShader->SetSides(GetSides());
		roundedBoxShader->SetFallOff(_fallOff);
		roundedBoxShader->SetCornerSize(_cornerSize);
	}*/
}

inline void RoundedBox::Draw() const
{
	//SquareVertices::Draw();  // Assuming this draws the box vertices
}
