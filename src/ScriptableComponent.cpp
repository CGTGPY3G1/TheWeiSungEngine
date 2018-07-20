#include "ScriptableComponent.h"
#include "ComponentHeaders.h"
namespace WeiSungEngine {
	ScriptableComponent::ScriptableComponent() {
	}

	ScriptableComponent::ScriptableComponent(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {

	}
}
