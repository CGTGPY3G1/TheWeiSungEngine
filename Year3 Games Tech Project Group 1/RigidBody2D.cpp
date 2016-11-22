#include "RigidBody2D.h"
#include "Math.h"
#include "GameObject.h"
#include <SFML/Graphics/Transform.hpp>
RigidBody2D::RigidBody2D(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {
	
}

RigidBody2D::~RigidBody2D() {

}

void RigidBody2D::AddForce(const Vector2 & force, const ForceType & forceType) {
	if(forceType == IMPULSE_FORCE)
		body->ApplyLinearImpulseToCenter(TypeConversion::ConvertToB2Vector2(force), true);
	else
		body->ApplyForceToCenter(TypeConversion::ConvertToB2Vector2(force), true);
}

void RigidBody2D::AddForceAtPoint(const Vector2 & force, const Vector2 & point, const ForceType & forceType) {
	if(forceType == IMPULSE_FORCE)
		body->ApplyLinearImpulse(TypeConversion::ConvertToB2Vector2(force), TypeConversion::ConvertToB2Vector2(point), true);
	else
		body->ApplyForce(TypeConversion::ConvertToB2Vector2(force), TypeConversion::ConvertToB2Vector2(point), true);
}

void RigidBody2D::AddTorque(const float & force, const ForceType & forceType) {
	if(forceType == IMPULSE_FORCE)
		body->ApplyAngularImpulse(force, true);
	else
		body->ApplyTorque(force, true);
}

Vector2 RigidBody2D::GetPosition() {
	return Vector2(TypeConversion::ConvertToVector2(body->GetPosition()));
}

void RigidBody2D::SetPosition(const Vector2 & newPosition) {
	body->SetTransform(TypeConversion::ConvertToB2Vector2(newPosition), body->GetAngle());
}

void RigidBody2D::SetRotation(const float & angle) {
	body->SetTransform(body->GetPosition(), angle * Math::DegreesToRadians());
}

Vector2 RigidBody2D::GetForward() {
	return Vector2(1, 0).RotatedInRadians(body->GetAngle());
}

Vector2 RigidBody2D::GetRight() {
	return GetForward().Flip90(true);
}

float RigidBody2D::GetRotation() {
	return body->GetAngle() * Math::RadiansToDegrees();
}

void RigidBody2D::SetMass(const float & newMass) {
	body->GetMassData(massData);
	massData->mass = newMass;
	body->SetMassData(massData);
}

float RigidBody2D::GetMass() {
	return body->GetMass();
}

Vector2 RigidBody2D::GetVelocity() {
	return TypeConversion::ConvertToVector2(body->GetLinearVelocity());
}

void RigidBody2D::SetVelocity(const Vector2 & newVelocity) {
	body->SetLinearVelocity(TypeConversion::ConvertToB2Vector2(newVelocity));
}

float RigidBody2D::GetSpeed() {
	return body->GetLinearVelocity().Length() * Physics::PIXELS_PER_METRE;
}

void RigidBody2D::Init(const b2BodyType & type, const float & mass, const float & angularDampening, const float & linearDampening) {
	bodyDef = new b2BodyDef();
	this->bodyDef->type = type;
	this->bodyDef->angularDamping = angularDampening;
	this->bodyDef->linearDamping = linearDampening;
	
	std::shared_ptr<GameObject> g = gameObject.lock();
	std::shared_ptr<Transform2D> t = g->GetComponent<Transform2D>().lock();
	bodyDef->position = TypeConversion::ConvertToB2Vector2(t->GetPosition());
	bodyDef->angle = t->GetRotation() * Math::DegreesToRadians();
	std::weak_ptr<Component> r = GetWeak();
	rigidBodyData = new RigidBodyData(r);
	bodyDef->userData = rigidBodyData;
	bodyDef->allowSleep = false;
	massData = new b2MassData();
	Message m = Message(MessageType::MESSAGE_TYPE_REGISTER_RIGIDBODY, MessageDataType::MESSAGE_RIGIDBODY_DATA_TYPE, rigidBodyData);
	g->HandleMessage(m);
	SetMass(mass);
	SetEnabled(true);
}
