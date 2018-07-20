#include "Collectable.h"
#include "TypeInfo.h"
#include "GameObject.h"
namespace WeiSungEngine {
	Collectable::Collectable() {
	}

	Collectable::Collectable(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {
	}

	Collectable::~Collectable() {
	}

	void Collectable::Init(const CollectableType & type, const unsigned int & amount) {
		cache.type = type; cache.amount = amount;
		if (cache.amount > 0) valid = true;
		else {
			valid = false;
			std::cout << "Invalid Collectable" << std::endl;
		}
	}

	CollectionCache Collectable::Activate() {
		if (valid) {
			gameObject.lock()->Destroy();
			valid = false;
		}
		return cache;
	}

	const bool Collectable::IsValid() const {
		return valid;
	}

}
