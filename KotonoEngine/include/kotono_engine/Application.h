#pragma once
class SApplication final
{
public:
	void Run();

private:
	bool isRunning_;

	void Init();
	void MainLoop();
	void Cleanup();
};

