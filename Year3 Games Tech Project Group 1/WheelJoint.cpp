#include "WheelJoint.h"
#include <Box2D\Dynamics\Joints\b2RevoluteJoint.h>
#include <Box2D\Dynamics\Joints\b2PrismaticJoint.h>
#include "GameObject.h"
#include "ComponentData.h"
#include "ComponentHeaders.h"
#include "Math.h"
WheelJoint::WheelJoint() : Joint() {
}

WheelJoint::WheelJoint(std::weak_ptr<GameObject> gameObject) : Joint(gameObject) {
}

WheelJoint::~WheelJoint() {
}

float WheelJoint::GetMotorSpeed() {
	return steering ? 0.0f : ((b2PrismaticJoint *)joint)->GetMotorSpeed();
}

void WheelJoint::SetMotorSpeed(const float & speed) {
	if(!steering) {
		((b2PrismaticJoint *)joint)->SetMotorSpeed(speed);
	}
}

void WheelJoint::SetTurnAngle(const float & degrees) {
	if(steering) {
		const float radians = degrees * Math::DegreesToRadians();
		((b2RevoluteJoint *)joint)->SetLimits(radians, radians);
	}
}

float WheelJoint::GetMaxTurnAngle() {
	return steering ? ((b2RevoluteJoint *)joint)->GetUpperLimit() * Math::RadiansToDegrees() : 0.0f;
}

float WheelJoint::GetAngle() {
	return  steering ? ((b2RevoluteJoint *)joint)->GetJointAngle() * Math::RadiansToDegrees() : 0.0f;
}


void WheelJoint::Init(const std::weak_ptr<RigidBody2D>& attatchedBody, const Vector2 & anchorPoint, const Vector2 & axis, const bool & enableMotor, const float & frequency, const float & maxTorque, const float & dampingRatio) {
	std::shared_ptr<RigidBody2D> m_Body = GetComponent<RigidBody2D>().lock();
	componentData = new ComponentData();
	componentData->comp = GetWeak();
	componentData->type = Type();
	b2Body * otherBody = attatchedBody.lock()->GetBody();
	if(!enableMotor) {
		b2RevoluteJointDef * revoluteJointDef = new b2RevoluteJointDef();
		revoluteJointDef->Initialize(otherBody, m_Body->GetBody(), m_Body->GetBody()->GetWorldCenter());
		//Vector2 scale = GetComponent<Transform2D>().lock()->GetScale();
		//Vector2 anchor = m_Body->GetPosition() + anchorPoint;
		//anchor -= m_Body->GetPosition();
		//b2Vec2 newAnchor =  revoluteJointDef->bodyB->GetLocalCenter();
		revoluteJointDef->lowerAngle = revoluteJointDef->upperAngle = 0.0f;
		revoluteJointDef->enableLimit = true;
		revoluteJointDef->collideConnected = false;
		jointDef = revoluteJointDef;
		steering = true;
	}
	else {
		b2PrismaticJointDef * prismaticJointDef = new b2PrismaticJointDef();
		prismaticJointDef->Initialize(otherBody, m_Body->GetBody(), m_Body->GetBody()->GetWorldCenter(), TypeConversion::ConvertToB2Vector2(axis));
		prismaticJointDef->enableLimit = true;
		prismaticJointDef->enableMotor = enableMotor;
		prismaticJointDef->motorSpeed = maxTorque;
		prismaticJointDef->collideConnected = false;
		prismaticJointDef->lowerTranslation = prismaticJointDef->upperTranslation = 0;
		jointDef = prismaticJointDef;
	}
	//revoluteJointDef->userData = componentData;
	
	Message m = Message(MessageScope::MESSAGE_SCOPE_PHYSICS_SYSTEM, MessageType::MESSAGE_TYPE_REGISTER_JOINT, componentData);
	gameObject.lock()->HandleMessage(m);
	SetEnabled(true);
}





