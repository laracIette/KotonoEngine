#include "RoundedBox.h"
#include "NDCRect.h"
#include "KotonoEngine.h"

float RoundedBox::GetFallOff() const
{
	return _fallOff;
}

float RoundedBox::GetCornerSize() const
{
	return _cornerSize;
}

float RoundedBox::GetTargetFallOff() const
{
	return _targetFallOff;
}

void RoundedBox::SetTargetFallOff(float targetFallOff)
{
	_targetFallOff = targetFallOff;
	UpdateValues();
}

float RoundedBox::GetTargetCornerSize() const
{
	return _targetCornerSize;
}

void RoundedBox::SetTargetCornerSize(float targetCornerSize)
{
	_targetCornerSize = targetCornerSize;
	UpdateValues();
}

void RoundedBox::SetRelativeSize(const glm::vec2& relativeSize)
{
	GetRect()->SetRelativeSize(relativeSize);
	UpdateValues();
}

void RoundedBox::SetWorldSize(const glm::vec2& worldSize)
{
	GetRect()->SetWorldSize(worldSize);
	UpdateValues();
}

void RoundedBox::UpdateShader() const
{
	Base::UpdateShader();
	/*GetShader()->SetUniform4f("sides", GetSides());
	GetShader()->SetUniform1f("fallOff", _fallOff);
	GetShader()->SetUniform1f("cornerSize", _cornerSize);*/
}

void RoundedBox::Draw() const
{
	Base::Draw();
	//_vertexArraySetup.Draw();
}

void RoundedBox::UpdateValues()
{
	glm::vec2 worldSize = GetRect()->GetWorldSize();

	float minSize = min(worldSize.x, worldSize.y);
	
	// CornerSize has a minimum of 0, max of half the smallest dimension
	_cornerSize = glm::clamp(_targetCornerSize, 0.0f, minSize * 0.5f);

	// FallOff has a minimum of 0
	_fallOff = max(0.0f, _targetFallOff);
}

const glm::mat4 RoundedBox::GetModelMatrix() const
{
	return NDCRect(
		GetRect()->GetWorldPosition(),
		GetRect()->GetWorldRotation(),
		_fallOff * 2.0f + GetRect()->GetWorldSize()
	).GetModelMatrix();
}

const glm::vec4 RoundedBox::GetSides() const
{
	glm::vec2 position(
		GetRect()->GetWorldPosition().x,
		Engine.GetWindow().GetSize().y - GetRect()->GetWorldPosition().y
	);
	glm::vec2 worldSize = GetRect()->GetWorldSize();

	return glm::vec4(
		position.x - 0.5f * worldSize.x,
		position.x + 0.5f * worldSize.x,
		position.y + 0.5f * worldSize.y,
		position.y - 0.5f * worldSize.y
	);
}
