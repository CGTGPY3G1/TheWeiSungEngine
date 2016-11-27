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
	graphics = std::make_shared<Graphics>();
	input = std::make_shared<Input>();
	timer = std::make_shared<Time>();
	debugDraw = new DebugDraw();
	if(game) game->Start();
}

void Engine::Run() {
	if(running) {
		if(graphics->GetWindowOpen()) {
			timer->Update();
			input->Update();
			graphics->Clear();
			graphics->Update();
			float deltaTime = timer->GetDeltaTime();
			if(running) {
				if(game) game->Update(timer->GetDeltaTime());
				else graphics->Draw("No Game Loaded!", Vector2(640.0f, 360.0f), 50, TextAlignment::CENTRE_ALIGNED);
			}
			graphics->Display();
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
}

void Engine::End() {

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

std::weak_ptr<Graphics> Engine::GetGraphics() {
	return graphics;
}

DebugDraw * Engine::GetDebugDraw() {
	return debugDraw;
}

std::weak_ptr<Input> Engine::GetInput() {
	return input;
}

std::weak_ptr<Time> Engine::GetTimer() {
	return timer;
}

unsigned int Engine::GetFPS() {
	return fps;
}
