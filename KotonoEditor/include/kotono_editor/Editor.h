#pragma once
class WWidget;
class SGameManager;
class SVisualizer;
class SCamera;
class SEditor final
{
public:
	void Init();
	void Update();
	void Cleanup();

public:
	SGameManager& GameManager();
	const SGameManager& GameManager() const;
	SVisualizer& Visualizer();
	const SVisualizer& Visualizer() const;
	SCamera& Camera();
	const SCamera& Camera() const;

private:
	WWidget* widget_;
};

inline SEditor Editor;