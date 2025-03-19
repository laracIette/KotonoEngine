#pragma once
class KtShader
{
public:
	virtual ~KtShader() = default;

	virtual void Init() = 0;
	virtual void Cleanup() = 0;
};

