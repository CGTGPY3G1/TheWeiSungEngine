#pragma once
#ifndef WS_TIME_H
#define WS_TIME_H

#include <chrono>
class Time {
public:
	Time();
	~Time();
	void Update();
	void End();
	float GetDeltaTime();
	float GetTotalTime();
private:
	std::chrono::steady_clock::time_point startTime, currentTime, lastFrameTime;
	float tempDelta = 0.0f, deltaTime = 0.0f, totalTime = 0.0f;
};

#endif // !WS_TIME_H