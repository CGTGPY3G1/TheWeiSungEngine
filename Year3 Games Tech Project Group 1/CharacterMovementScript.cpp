#include "CharacterMovementScript.h"
#include "GameObject.h"
CharacterMovementScript::CharacterMovementScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

CharacterMovementScript::~CharacterMovementScript() {
}

void CharacterMovementScript::Start() {
	rigidbody = GetComponent<RigidBody2D>();
	transform = GetComponent<Transform2D>();
}

void CharacterMovementScript::MoveUsingPhysics(Vector2 & force, const bool & worldSpace) {
	std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
	if(rb) {
		if(worldSpace) {
			const float scale = 1.0f + std::max<float>(transform.lock()->GetForward().Dot(force.Normalized()), 0.25f);
			rb->AddForce(force * scale);
		}
		else {
			const float magnitude = force.Magnitude();
			Vector2 direction = transform.lock()->TransformToLocalDirection(force);
			const float scale = 1.0f + std::max<float>(transform.lock()->GetForward().Dot(direction), 0.25f);
			rb->AddForce(direction * magnitude * scale);
		}
	}
}

void CharacterMovementScript::Move(Vector2 & amount, const bool & worldSpace) {
	std::shared_ptr<Transform2D> t = transform.lock();
	if(worldSpace) {
		t->Move(amount);
	}
	else {
		const float magnitude = amount.Magnitude();
		Vector2 direction = transform.lock()->TransformToLocalDirection(amount);
		const float scale = 1.0f + std::max<float>(transform.lock()->GetForward().Dot(direction), 0.4f);
		t->Move(direction * magnitude * scale);
	}
}