#include "Game.h"
#include "Scene.h"
#include <iostream>
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
}

void Game::Update(const float & deltaTime) {
	if(running && !gameOver) {
		if(activeScene < scenes.size()) {
			scenes[activeScene]->Update(deltaTime);
		}
	}
}

void Game::Close() {
}

void Game::AddScene(Scene * scene) {
	std::vector<Scene *>::iterator it = std::find(scenes.begin(), scenes.end(), scene);
	if(it == scenes.end()) scenes.push_back(scene);
	else std::cout << "Scene Already Added!" << std::endl;
}

unsigned int Game::GetActiveScene() {
	return activeScene;
}

void Game::SetActiveScene(unsigned int index) {
	if(index < activeScene) {
		activeScene = index;
	}
}