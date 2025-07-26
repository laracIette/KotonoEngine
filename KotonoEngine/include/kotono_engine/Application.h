#pragma once
class SApplication final
{
public:
	static void Run();

private:
	void Init();
	void MainLoop();
	void Cleanup();
};

