#pragma once
class WWidget;
class SVisualizer;
class SCamera;
class SEditor final
{
public:
	void Init();
	void Update();
	void Cleanup();

public:
	SVisualizer& Visualizer();
	const SVisualizer& Visualizer() const;
	SCamera& Camera();
	const SCamera& Camera() const;

private:
	WWidget* widget_;
};

inline SEditor Editor;