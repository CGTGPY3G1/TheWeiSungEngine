#include "ScriptManagementSystem.h"
#include "GameObject.h"
#include "ScriptableComponent.h"
namespace WeiSungEngine {
	bool SortScripts(const std::shared_ptr<ScriptableComponent> & l, const std::shared_ptr<ScriptableComponent> & r) {
		return l->GetSortOrder() < r->GetSortOrder();
	}

	ScriptManagementSystem::ScriptManagementSystem() {
	}

	ScriptManagementSystem::~ScriptManagementSystem() {
	}

	void ScriptManagementSystem::UnloadScripts() {
		updatable.clear();
		fixedUpdatable.clear();
		lateUpdatable.clear();
		renderable.clear();
	}

	void ScriptManagementSystem::LoadScripts(std::vector<std::shared_ptr<GameObject>>& gameObjects) {
		UnloadScripts();

		const size_t noOfObjects = gameObjects.size();
		for (size_t i = 0; i < noOfObjects; i++) {
			std::shared_ptr<GameObject> go = gameObjects[i];
			if (go) {
				if (go->GetEnabled()) {
					std::vector<std::shared_ptr<ScriptableComponent>> newScripts = go->GetScriptableComponents();
					if (!newScripts.empty()) {
						for (std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = newScripts.begin(); i != newScripts.end(); ++i) {
							std::shared_ptr<ScriptableComponent> c = (*i);
							const ComponentType type = c->Type();
							if (TypeInfo::IsUpdatable(type)) updatable.push_back(c);
							if (TypeInfo::IsFixedUpdatable(type)) fixedUpdatable.push_back(c);
							if (TypeInfo::IsLateUpdatable(type)) lateUpdatable.push_back(c);
							if (TypeInfo::IsRenderable(type)) renderable.push_back(c);
						}
					}
				}
			}
		}
		std::sort(updatable.begin(), updatable.end(), SortScripts);
		std::sort(fixedUpdatable.begin(), fixedUpdatable.end(), SortScripts);
		std::sort(lateUpdatable.begin(), lateUpdatable.end(), SortScripts);
		std::sort(renderable.begin(), renderable.end(), SortScripts);

	}

	void ScriptManagementSystem::FixedUpdate(const float & fixedDeltaTime) {
		for (std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = fixedUpdatable.begin(); i != fixedUpdatable.end(); ++i) {
			std::shared_ptr<ScriptableComponent> script = (*i);
			if (script) {
				if (script->GetEnabled()) script->FixedUpdate(fixedDeltaTime);
			}
		}
	}

	void ScriptManagementSystem::Update(const float & deltaTime) {
		for (std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = updatable.begin(); i != updatable.end(); ++i) {
			std::shared_ptr<ScriptableComponent> script = (*i);
			if (script) {
				if (script->GetEnabled()) script->Update(deltaTime);
			}
		}
	}

	void ScriptManagementSystem::Render() {
		for (std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = renderable.begin(); i != renderable.end(); ++i) {
			std::shared_ptr<ScriptableComponent> script = (*i);
			if (script) {
				if (script->GetEnabled()) script->Render();
			}
		}
	}

	void ScriptManagementSystem::LateUpdate() {
		for (std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = lateUpdatable.begin(); i != lateUpdatable.end(); ++i) {
			std::shared_ptr<ScriptableComponent> script = (*i);
			if (script) {
				if (script->GetEnabled()) script->LateUpdate();
			}
		}
	}
}
