#include "GameObjectManager.h"
#include "Scene.h"
GameObjectManager::GameObjectManager() {

}

GameObjectManager & GameObjectManager::GetInstance() {
	static GameObjectManager instance;
	return instance;
}

GameObjectManager::~GameObjectManager() {

}


std::weak_ptr<GameObject> GameObjectManager::CreateGameObject(const std::string & name) {
	this->gameObjects;
	unsigned int id;
	if(!freeIDs.empty()) {
		id = freeIDs.back();
		freeIDs.pop_back();
	}
	else id = nextID++;
	std::shared_ptr<GameObject> g = std::make_shared<GameObject>(id, name);
	gameObjects.push_back(g);
	return g;
}

std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjects() {
	return gameObjects;
}


std::weak_ptr<GameObject> GameObjectManager::GetGameObject(const unsigned int & id) {
	std::vector<std::shared_ptr<GameObject>>::iterator it = std::find_if(gameObjects.begin(), gameObjects.end(), HasGameObjectID(id));
	if(it != gameObjects.end()) return (*it);
	return std::weak_ptr<GameObject>();
}

std::weak_ptr<GameObject> GameObjectManager::GetGameObject(const std::string & name) {
	std::vector<std::shared_ptr<GameObject>>::iterator it = std::find_if(gameObjects.begin(), gameObjects.end(), HasName(name));
	if(it != gameObjects.end()) return (*it);
	return std::weak_ptr<GameObject>();
}

std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjectsWithName(const std::string & name) {
	std::vector<std::shared_ptr<GameObject>> toReturn = std::vector<std::shared_ptr<GameObject>>();
	for(std::vector<std::shared_ptr<GameObject>>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)	if((*it)->name.compare(name) == 0) toReturn.push_back(*it);
	return toReturn;
}

std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjectsWithComponent(const unsigned int & componentMask) {
	std::vector<std::shared_ptr<GameObject>> toReturn = std::vector<std::shared_ptr<GameObject>>();
	for(std::vector<std::shared_ptr<GameObject>>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)	if((*it)->HasComponents(componentMask)) toReturn.push_back(*it);
	return toReturn;
}

void GameObjectManager::DeleteGameObject(const unsigned int & id) {
	std::vector<std::shared_ptr<GameObject>>::iterator it = std::find_if(gameObjects.begin(), gameObjects.end(), HasGameObjectID(id));
	if(it != gameObjects.end()) gameObjects.erase(it);
}

