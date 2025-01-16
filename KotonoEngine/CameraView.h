#pragma once
class CameraView
{
public:
	const float GetAspectRatio() const;

	void SetAspectRatio(const float aspectRatio);

private:
	float _aspectRatio;
	float _fov; // vertical
};

