#include "Engine.h"
#include "Input.h"
#include "Graphics.h"
#include "Time.h"
#include "DebugDraw.h"
#include "Game.h"

Engine::Engine() {
}

Engine::~Engine() {
}

Engine & Engine::GetInstance() {
	static Engine instance;
	return instance;
}

void Engine::Start() {
	End();
	running = true;
	graphics = new Graphics();
	input = new Input();
	timer = new Time();
	debugDraw = new DebugDraw();
	if(game) game->Start();
}

void Engine::Run() {
	if(graphics->GetWindowOpen()) {
		timer->Update();
		input->Update();
		graphics->Clear();
		float deltaTime = timer->GetDeltaTime();
		if(running) {
			if(game) game->Update(timer->GetDeltaTime());
			else graphics->Draw("No Game Loaded!", Vector2(640.0f, 360.0f), 50, TextAlignment::CENTRE_ALIGNED);
		}
		graphics->Update();
		framesThisSecond++;
		fpsTimer += deltaTime;
		if(fpsTimer > 1.0f) {
			fps = framesThisSecond; 
			framesThisSecond = 0;
			fpsTimer = 0.0f;
		}
	}
	else running = false;
}

void Engine::End() {
	if(graphics) {
		delete graphics;
		graphics = nullptr;
	}
	if(input) {
		delete input;
		input = nullptr;
	}
	if(timer) {
		delete timer;
		timer = nullptr;
	}
	if(debugDraw) {
		delete debugDraw;
		debugDraw = nullptr;
	}
}

void Engine::Close() {
	End();
	running = false;
}

bool Engine::IsRunning() {
	return running;
}

void Engine::SetGame(std::shared_ptr<Game> newGame) {
	game = newGame;
	if(game) game->Start();
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

unsigned int Engine::GetFPS() {
	return fps;
}
