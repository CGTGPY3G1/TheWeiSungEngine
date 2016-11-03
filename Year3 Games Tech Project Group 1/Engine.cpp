#include "Engine.h"
#include "Input.h"
#include "Graphics.h"
#include "Time.h"
#include "DebugDraw.h"
Engine::Engine() {
	Init();
}

Engine::~Engine() {
}

Engine & Engine::GetInstance() {
	static Engine instance;
	return instance;
}

void Engine::Init() {
	running = true;
	graphics = new Graphics();
	input = new Input();
	timer = new Time();
	debugDraw = new DebugDraw();
}

void Engine::Run() {

}

void Engine::Close() {
}

bool Engine::IsRunning() {
	return running;
}

Graphics * Engine::GetGraphics() {
	return graphics;
}

DebugDraw * Engine::GetDebugDraw() {
	return debugDraw;
}

Input * Engine::GetInput() {
	return input;
}

Time * Engine::GetTimer() {
	return timer;
}

FileIO * Engine::GetFileIO() {
	return fileIO;
}
