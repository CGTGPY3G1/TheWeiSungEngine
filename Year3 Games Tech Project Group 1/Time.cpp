#include "Time.h"

Time::Time() {
	startTime = std::chrono::steady_clock::now();
	currentTime = startTime;
	lastFrameTime = startTime;
	deltaTime = 0.0f;
	totalTime = deltaTime;
}

Time::~Time() {
}

void Time::Update() {
	currentTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::duration step = (currentTime - lastFrameTime);
	float stepAsFloat = float(step.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
	this->deltaTime = stepAsFloat;
	this->totalTime += stepAsFloat;
	lastFrameTime = currentTime;
}

float Time::GetDeltaTime() {
	return deltaTime;
}

float Time::GetTotalTime() {
	return totalTime;
}

void Time::End() {}