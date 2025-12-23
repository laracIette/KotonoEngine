#pragma once
#include <kotono_common/Ptr.h>
#undef interface
class KScene;
class KInterface;
class SGame final
{
	friend class SCore;
	friend class STimeManager;

private:
	void Init();
	void Update();

public:
	void OpenScene(const UPtr<KScene>& scene);
	void OpenInterface(const UPtr<KInterface>& interface);

	const UPtr<KScene>& GetOpenedScene() const;

private:
	UPtr<KScene> scene_;
	UPtr<KInterface> interface_;

	void OnKeySPressed() const;
};

inline SGame Game;
