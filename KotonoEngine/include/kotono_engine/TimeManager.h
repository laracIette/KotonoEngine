#pragma once
class STimeManager final
{
	friend class SEngine;

private:
	void Init();
	void Update();
	void Cleanup();

public:
	// Current exact UTC Time since Epoch in seconds.
	double GetExactUTC() const;
	// Current Time since the start of the program in seconds.
	float GetNow() const;
	// Delta Time in seconds.
	float GetDelta() const;

private:
	double _startUTC;
	float _nowTime;             
	float _previousTime;        
	float _deltaTime;           
};

