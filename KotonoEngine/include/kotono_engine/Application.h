#pragma once
class SApplication
{
public:
	void Run();

private:
	bool isRunning_;

	void Init();
	void MainLoop();
	void Cleanup();
};

