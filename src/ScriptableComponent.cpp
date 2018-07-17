#include "ScriptableComponent.h"
#include "ComponentHeaders.h"
ScriptableComponent::ScriptableComponent() {
}

ScriptableComponent::ScriptableComponent(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {

}

