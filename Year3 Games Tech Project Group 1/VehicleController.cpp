#include "VehicleController.h"
#include "RigidBody2D.h"
#include "Transform2D.h"
#include "GameObject.h"
VehicleController::VehicleController() {
}

VehicleController::VehicleController(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

VehicleController::~VehicleController() {
}

void VehicleController::Start() {
	myTransform = GetComponent<Transform2D>();
	rigidbody = GetComponent<RigidBody2D>();
}

void VehicleController::FixedUpdate(const float & fixedDeltaTime) {
	std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
	if(rb) {
		Vector2 lateralImpulse = GetLateralVelocity(rb);
		rb->AddForce(-lateralImpulse * 0.2f, ForceType::IMPULSE_FORCE);
		rb->AddTorque(0.2f * rb->GetInertia() * -rb->GetAngularVelocity(), ForceType::IMPULSE_FORCE);
	}
	else {
		rigidbody = GetComponent<RigidBody2D>();
	}
}

void VehicleController::Drive(const float & force) {
	std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
	if(rb) rb->AddForce(rb->GetForward() * force, ForceType::IMPULSE_FORCE);
}

void VehicleController::Steer(const float & steerValue) {
	std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
	if(rb) {
		float dot = rb->GetVelocity().Dot(rb->GetForward());
		dot = (dot < 0.0f) ? -1.0f : 1.0f;
		const float massScale = Physics::METRES_PER_PIXEL * rb->GetSpeed() * 3.0f;
		rb->AddTorque(steerValue * dot * massScale, ForceType::IMPULSE_FORCE);
		rb->AddTorque(steerValue * dot * massScale, ForceType::IMPULSE_FORCE);
	}
	else {
		rigidbody = GetComponent<RigidBody2D>();
	}
}

Vector2 VehicleController::GetLateralVelocity(std::shared_ptr<RigidBody2D> r) {
	Vector2 currentRightNormal = r->GetRight();
	return currentRightNormal * currentRightNormal.Dot(r->GetVelocity());
}