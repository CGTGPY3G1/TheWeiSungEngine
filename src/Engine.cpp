#include "Engine.h"
#include "Input.h"
#include "Graphics.h"
#include "Time.h"
#include "DebugDraw.h"
#include "Game.h"
#include "AssetManager.h"
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
	sf::Sprite title(AssetManager::GetInstance().GetTexture("Images/Title.png"));
	Vector2 res = graphics->GetScreenResolution();
	sf::FloatRect rect = title.getGlobalBounds();
	title.setOrigin(sf::Vector2f(rect.width * 0.5f, rect.height * 0.5f));
	title.setPosition(res * 0.5f);
	const float zoom = graphics->GetCameraZoom();
	title.scale(sf::Vector2f(zoom, zoom));
	graphics->Clear();
	graphics->Draw(title);
	graphics->Display();
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
				//else graphics->Draw("No Game Loaded!", Vector2(640.0f, 360.0f), 50, TextAlignment::CENTRE_ALIGNED);
			}
			graphics->Display();
			framesThisSecond++;
			fpsTimer += deltaTime;
			if(fpsTimer > 1.0f) {
				fps = framesThisSecond;
				//std::cout << "FPS = " + std::to_string(fps) << std::endl;
				framesThisSecond = 0;
				fpsTimer -= 1.0f;
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
