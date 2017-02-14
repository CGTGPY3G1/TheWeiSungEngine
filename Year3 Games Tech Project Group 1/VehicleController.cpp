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
		const float dampeningForce = 0.6f;
		rb->AddForce(-lateralImpulse * dampeningForce * 2.0f, ForceType::IMPULSE_FORCE);
		rb->AddTorque(dampeningForce * rb->GetInertia() * -rb->GetAngularVelocity(), ForceType::IMPULSE_FORCE);
	}
	else {
		rigidbody = GetComponent<RigidBody2D>();
	}
}

void VehicleController::Drive(const float & force) {
	std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
	if(rb) {
		if((rb->GetVelocity() * Physics::METRES_PER_PIXEL).SquareMagnitude() < squaredMaxSpeed) rb->AddForce(rb->GetForward() * force * accelerationForce * rb->GetMass(), ForceType::FORCE);
	}
}

void VehicleController::SetAccelerationForce(const float & force) {
	accelerationForce = force;
}

float VehicleController::GetAccelerationForce() {
	return accelerationForce;
}

void VehicleController::SetSteeringForce(const float & force) {
	steeringForce = force;
}

float VehicleController::GetSteeringForce() {
	return steeringForce;
}

void VehicleController::SetMaxSpeed(const float & speed) {
	maxSpeed = speed;
	squaredMaxSpeed = speed * speed;
}

float VehicleController::GetMaxSpeed() {
	return maxSpeed;
}

void VehicleController::Steer(const float & steerValue) {
	std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
	if(rb) {
		float dot = rb->GetVelocity().Dot(rb->GetForward());
		dot = (dot < 0.0f) ? -1.0f : 1.0f;
		const float massScale = Physics::METRES_PER_PIXEL * rb->GetSpeed() * rb->GetMass();
		rb->AddTorque(steerValue * dot * steeringForce * massScale, ForceType::FORCE);
		//rb->AddTorque(steerValue * dot * massScale, ForceType::FORCE);
	}
	else {
		rigidbody = GetComponent<RigidBody2D>();
	}
}

void VehicleController::OnCollisionEnter(const CollisionData & data) {
	std::shared_ptr<GameObject> g = data.gameObject.lock();
	if(g) {
		std::shared_ptr<HealthScript> hs = g->GetComponent<HealthScript>().lock();
		std::shared_ptr<HealthScript> mhs = GetComponent<HealthScript>().lock();
		std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
		std::shared_ptr<RigidBody2D> rb2 = g->GetComponent<RigidBody2D>().lock();
		const float relativeVelocity = (rb2->GetVelocity() - rb->GetVelocity()).Magnitude() * Physics::METRES_PER_PIXEL;
		if(relativeVelocity > 10.0f) {
			if(hs) hs->AddToHealth(-Random::RandomFloat(relativeVelocity, relativeVelocity * 2));
			if(mhs) mhs->AddToHealth(-Random::RandomFloat(relativeVelocity, relativeVelocity * 2));
		}
		
		
	}
}

int VehicleController::GetSortOrder() {
	const static int order = TypeInfo::ScriptSortOrder<VehicleController>();
	return order;
}

Vector2 VehicleController::GetLateralVelocity(std::shared_ptr<RigidBody2D> r) {
	Vector2 currentRightNormal = r->GetRight();
	return currentRightNormal * currentRightNormal.Dot(r->GetVelocity());
}
