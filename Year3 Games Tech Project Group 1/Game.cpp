#include "Game.h"
#include "Scene.h"
#include <iostream>
#include "Engine.h"
#include "Graphics.h"
#include "Time.h"

Game::Game() {
}

Game::~Game() {
}

bool Game::IsRunning() {
	return running;
}

bool Game::GameOver() {
	return gameOver;
}

void Game::Start() {
	running = true;
	gameOver = false;
}

void Game::Update(const float & deltaTime) {
	if(running && !gameOver) {
		if(activeScene >= 0 && activeScene < (int)scenes.size()) {
			float fixedDelta = Engine::GetInstance().GetTimer().lock()->GetFixedDeltaTime();
			std::shared_ptr<Scene> scene = scenes[activeScene];
			accumulator += deltaTime;
			while(accumulator >= fixedDelta) {
				scene->FixedUpdate(fixedDelta);
				accumulator -= fixedDelta;
			}
			
			scene->Update(deltaTime);
			scene->Render();
			scene->FinaliseFrame();
		}
		else Engine::GetInstance().GetGraphics().lock()->Draw("No Scene Loaded!", Vector2(640.0f, 360.0f), 50, TextAlignment::CENTRE_ALIGNED);
	}
}

void Game::Close() {
}

void Game::AddScene(std::weak_ptr<Scene> scene) {
	std::shared_ptr<Scene> newScene = scene.lock();
	if(newScene) {
		std::vector<std::shared_ptr<Scene>>::iterator it = std::find(scenes.begin(), scenes.end(), newScene);
		if(it == scenes.end()) { 
			scenes.push_back(newScene); 
		}
		else std::cout << "Scene Already Added!" << std::endl;
	}
	else std::cout << "Scene is invalid" << std::endl;
}

int Game::GetActiveScene() {
	return activeScene;
}

void Game::SetActiveScene(unsigned int index) {
	if(activeScene != index) {
		if(index < scenes.size()) activeScene = index;
		if(index >= 0) scenes[activeScene]->Start();
		
	}
}