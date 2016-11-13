#include "ScriptableComponent.h"

ScriptableComponent::ScriptableComponent(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {
	SetEnabled(true);
}
