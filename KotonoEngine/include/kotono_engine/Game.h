#pragma once
#include <kotono_engine/Ptr.h>
class KScene;
class KInterface;
class SGame final
{
	friend class SEngine;
	friend class STimeManager;

private:
	void Init();
	void Update();

public:
	void OpenScene(const UPtr<KScene>& scene);
	void OpenInterface(const UPtr<KInterface>& interface);

private:
	UPtr<KScene> scene_;
	UPtr<KInterface> interface_;
};

