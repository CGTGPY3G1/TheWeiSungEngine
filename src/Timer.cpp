#include "Timer.h"
#include "EngineSettings.h"
namespace WeiSungEngine {
	Timer::Timer() {
		startTime = std::chrono::steady_clock::now();
		currentTime = startTime;
		lastFrameTime = startTime;
		deltaTime = 0.0f;
		totalTime = deltaTime;
	}

	Timer::~Timer() {
	}

	void Timer::Update() {
		currentTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::duration step = (currentTime - lastFrameTime);
		float stepAsFloat = float(step.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
		this->deltaTime = stepAsFloat;
		this->totalTime += stepAsFloat;
		lastFrameTime = currentTime;
	}

	float Timer::GetDeltaTime() {
		return deltaTime * EngineSettings::TIME_SCALE;
	}

	float Timer::GetFixedDeltaTime() {
		return FIXED_DELTA * EngineSettings::TIME_SCALE;
	}

	float Timer::GetTotalTime() {
		return totalTime;
	}

	void Timer::End() {}
}
