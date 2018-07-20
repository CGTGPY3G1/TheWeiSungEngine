#include "Joint.h"
namespace WeiSungEngine {
	Joint::Joint() : Component() {
	}
	Joint::Joint(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {

	}

	Joint::~Joint() {
	}
}
