#include "VehicleController.h"
#include "GameObjectManager.h"
#include "TileMapper.h"
#include "RigidBody2D.h"
#include "Transform2D.h"
#include "GameObject.h"
#include "CircleCollider.h"
#include "Engine.h"
#include "Graphics.h"
VehicleController::VehicleController() {
}

VehicleController::VehicleController(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

VehicleController::~VehicleController() {
}

void VehicleController::Start() {

	std::shared_ptr<Transform2D> transform = GetComponent<Transform2D>().lock();
	myTransform = transform;
	rigidbody = GetComponent<RigidBody2D>();
	GameObjectManager & goManager = GameObjectManager::GetInstance();
	Vector2 position = transform->GetPosition(), right = transform->GetRight(), forward = transform->GetForward();
	for(size_t i = 0; i < 4; i++) {
		std::shared_ptr<GameObject> wheel = goManager.CreateGameObject("Wheel").lock();
		wheel->Init(position + (right * wheelsOffsets[i].x) + (forward * wheelsOffsets[i].y), transform->GetRotation());
		std::shared_ptr<Transform2D> wheelTransform = wheel->GetComponent<Transform2D>().lock();
		wheelTransform->SetParent(transform);
		wheels[i] = wheelTransform;
	}	
	std::shared_ptr<GameObject> tm = goManager.GetGameObject("Tileset").lock();
	if(tm) tileMapper = tm->GetComponent<TileMapper>();
}

void VehicleController::FixedUpdate(const float & fixedDeltaTime) {
	std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
	if(rb) {
		Vector2 lateralImpulse = GetVelocityDampening(rb);
		const float dampeningForce = 0.6f;
		rb->AddForce(-lateralImpulse * dampeningForce * 2.0f, ForceType::IMPULSE_FORCE);
		rb->AddTorque(dampeningForce * rb->GetInertia() * -rb->GetAngularVelocity(), ForceType::IMPULSE_FORCE);
	}
	else {
		rigidbody = GetComponent<RigidBody2D>();
	}
}

void VehicleController::Render() {
	std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
	for(int i = 0; i < 4; i++) {
		sf::Vertex verts[2];
		verts[0].color = sf::Color::Red;
		std::shared_ptr<Transform2D> wTransform = wheels[i].lock();
		verts[0].position = wTransform->GetPosition();
		verts[1].color = sf::Color::Red;
		verts[1].position = wTransform->GetPosition() + wTransform->GetForward() * 50;
		graphics->DrawLine(verts);
		sf::Vertex verts2[2];
		verts2[0].color = sf::Color::Green;
		verts2[0].position = wTransform->GetPosition();
		verts2[1].color = sf::Color::Green;
		verts2[1].position = wTransform->GetPosition() + wTransform->GetRight() * 50;
		graphics->DrawLine(verts2);
	}
}

void VehicleController::Drive(const float & force) {
	std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
	if(rb) {
		if((rb->GetVelocity() * Physics::METRES_PER_PIXEL).SquareMagnitude() < squaredMaxSpeed) {
			const Vector2 lw = wheels[Wheel::FrontLeft].lock()->GetPosition(), rw = wheels[Wheel::FrontRight].lock()->GetPosition(), blw = wheels[Wheel::BackLeft].lock()->GetPosition(), brw = wheels[Wheel::BackRight].lock()->GetPosition();
			const float tileForceScaleFL = GetForceScale(lw), tileForceScaleFR = GetForceScale(rw), tileForceScaleBL = GetForceScale(lw), tileForceScaleBR = GetForceScale(rw);
			const float tileForceScale = (tileForceScaleFL + tileForceScaleBL + tileForceScaleFR + tileForceScaleBR) * 0.25f;
			rb->AddForce(rb->GetForward() * force * accelerationForce * rb->GetMass() * tileForceScale, ForceType::FORCE);
			//rb->AddForceAtPoint(rb->GetForward() * force * accelerationForce * rb->GetMass() * tileForceScaleR, rw, ForceType::FORCE);
		}
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
		const Vector2 lw = wheels[Wheel::FrontLeft].lock()->GetPosition(), rw = wheels[Wheel::FrontRight].lock()->GetPosition();
		const float tileForceScaleL = GetForceScale(lw), tileForceScaleR = GetForceScale(rw);
		const float tileForceScale = (tileForceScaleL + tileForceScaleR) * 0.5f;
		float dot = rb->GetVelocity().Dot(rb->GetForward());
		dot = (dot < 0.0f) ? -1.0f : 1.0f;
		const float massScale = Physics::METRES_PER_PIXEL * rb->GetSpeed() * rb->GetMass();
		rb->AddTorque(steerValue * dot * steeringForce * tileForceScale * massScale, ForceType::FORCE);
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

float VehicleController::GetForceScale(const Vector2 & worldPosition) {
	float tileForceScale = 1.0f;
	if(!tileMapper.expired()) tileForceScale = tileMapper.lock()->GetTileForceScale(worldPosition);
	else {
		std::shared_ptr<GameObject> tm = GameObjectManager::GetInstance().GetGameObject("Tileset").lock();
		if(tm) tileMapper = tm->GetComponent<TileMapper>();
	}
	return tileForceScale;
}

Vector2 VehicleController::GetVelocityDampening(std::shared_ptr<RigidBody2D> r) {
	Vector2 currentRightNormal = r->GetRight();
	Vector2 velocity = r->GetVelocity();
	return (currentRightNormal * currentRightNormal.Dot(velocity)) + (velocity * (1.0f - GetForceScale(r->GetPosition())));
}
