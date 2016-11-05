#include "GameObjectManager.h"
#include "Scene.h"
GameObjectManager::GameObjectManager(std::weak_ptr<Scene> owner) :scene(owner) {

}

GameObjectManager::~GameObjectManager() {

}

std::weak_ptr<GameObject> GameObjectManager::CreateGameObject() {
	unsigned int id;
	if(!freeIDs.empty()) {
		id = freeIDs.back();
		freeIDs.pop_back();
	}
	else id = nextID++;
	std::shared_ptr<GameObject> g = std::make_shared<GameObject>(GetWeak(), id);
	gameObjects.push_back(g);
	return g;
}

std::weak_ptr<GameObject> GameObjectManager::CreateGameObject(const std::string name) {
	unsigned int id;
	if(!freeIDs.empty()) {
		id = freeIDs.back();
		freeIDs.pop_back();
	}
	else id = nextID++;
	std::shared_ptr<GameObject> g = std::make_shared<GameObject>(GetWeak(), id, name);
	gameObjects.push_back(g);
	return std::weak_ptr<GameObject>();
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

std::weak_ptr<Scene> GameObjectManager::GetScene() {
	return scene;
}
