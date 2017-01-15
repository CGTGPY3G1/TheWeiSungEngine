#include "ScriptManagementSystem.h"
#include "GameObject.h"
#include "ScriptableComponent.h"
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
}

void ScriptManagementSystem::FixedUpdate(const float & fixedDeltaTime) {
	const size_t noOfObjects = scripts.size();
	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<ScriptableComponent> script = scripts[i];
		if(script) {
			script->FixedUpdate(fixedDeltaTime);
		}
	}
}

void ScriptManagementSystem::Update(const float & deltaTime) {
	const size_t noOfObjects = scripts.size();
	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<ScriptableComponent> script = scripts[i];
		if(script) {
			script->Update(deltaTime);
		}
	}
}

void ScriptManagementSystem::Render() {
}

void ScriptManagementSystem::LateUpdate() {
	const size_t noOfObjects = scripts.size();
	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<ScriptableComponent> script = scripts[i];
		if(script) {
			script->LateUpdate();
		}
	}
}
