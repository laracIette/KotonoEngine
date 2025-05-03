#pragma once
class KtEventBase
{
public:
	virtual ~KtEventBase() = default;

	virtual void RemoveListener(void* instance) = 0;
};

