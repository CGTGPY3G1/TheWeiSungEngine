#pragma once
#ifndef WS_COLLECTABLE_H
#define WS_COLLECTABLE_H
#include "ScriptableComponent.h"

enum CollectableType {
	CollectablePistol = 0,
	CollectableUzi = 1,
	CollectableGrenade = 2,
	CollectableHealth = 3
};

struct CollectionCache {
	CollectableType type;
	unsigned int amount;
};

class Collectable : public Component {
public:
	Collectable();
	Collectable(std::weak_ptr<GameObject> gameObject);
	~Collectable();
	const ComponentType Type() const override { return COMPONENT_COLLECTABLE; }
	const std::string GetName() const override { return "Collectable"; }
	void Init(const CollectableType & type, const unsigned int & amount);
	CollectionCache Activate();
	const bool IsValid() const;
private:
	CollectionCache cache;
	bool valid = false;
};
#endif // !WS_COLLECTABLE_H