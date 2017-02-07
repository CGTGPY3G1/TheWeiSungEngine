#include "ScriptManagementSystem.h"
#include "GameObject.h"
#include "ScriptableComponent.h"


bool SortScripts(const std::shared_ptr<ScriptableComponent> & l, const std::shared_ptr<ScriptableComponent> & r) {
	return l->GetSortOrder() < r->GetSortOrder();
}

ScriptManagementSystem::ScriptManagementSystem() {
}

ScriptManagementSystem::~ScriptManagementSystem() {
}

void ScriptManagementSystem::UnloadScripts() {
	scripts.clear();
}

void ScriptManagementSystem::LoadScripts(std::vector<std::shared_ptr<GameObject>>& gameObjects) {
	UnloadScripts();
	const size_t noOfObjects = gameObjects.size();
	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<GameObject> go = gameObjects[i];
		if(go) {		
			if(go->GetEnabled()) {
				std::vector<std::shared_ptr<ScriptableComponent>> newScripts = go->GetScriptableComponents();
				if(!newScripts.empty()) scripts.insert(scripts.end(), newScripts.begin(), newScripts.end());
			}		
		}
	}
	std::sort(scripts.begin(), scripts.end(), SortScripts);
}

void ScriptManagementSystem::FixedUpdate(const float & fixedDeltaTime) {
	const size_t noOfObjects = scripts.size();
	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<ScriptableComponent> script = scripts[i];
		if(script) {
			if(script->GetEnabled()) script->FixedUpdate(fixedDeltaTime);
		}
	}
}

void ScriptManagementSystem::Update(const float & deltaTime) {
	const size_t noOfObjects = scripts.size();
	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<ScriptableComponent> script = scripts[i];
		if(script) {
			if(script->GetEnabled()) script->Update(deltaTime);
		}
	}
}

void ScriptManagementSystem::Render() {
	const size_t noOfObjects = scripts.size();
	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<ScriptableComponent> script = scripts[i];
		if(script) {
			if(script->GetEnabled()) script->Render();
		}
	}
}

void ScriptManagementSystem::LateUpdate() {
	const size_t noOfObjects = scripts.size();
	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<ScriptableComponent> script = scripts[i];
		if(script) {
			if(script->GetEnabled()) script->LateUpdate();
		}
	}
}
