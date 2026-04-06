#pragma once
#include <kotono_object/Ptr.h>
#undef interface
class KInterface;
class KScene;
class SGame final
{
	friend class SCore;
	friend class STimeManager;

private:
	void Init();
	void Update(const float deltaTime);
	void Cleanup();

public:
	void OpenInterface(const UPtr<KInterface>& interface);
	void OpenScene(const UPtr<KScene>& scene);

	void OpenStartupInterface();
	void OpenStartupScene();

	const UPtr<KScene>& GetOpenedScene() const;

private:
	void OnKeySPressed() const;

private:
	UPtr<KScene> scene_;
	UPtr<KInterface> interface_;
};

inline SGame Game;
