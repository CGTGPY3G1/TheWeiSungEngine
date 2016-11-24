#include "WheelJoint.h"
#include <Box2D\Dynamics\Joints\b2WheelJoint.h>
#include "GameObject.h"
#include "ComponentData.h"
#include "ComponentHeaders.h"

WheelJoint::WheelJoint() : Joint() {
}

WheelJoint::WheelJoint(std::weak_ptr<GameObject> gameObject) : Joint(gameObject) {
}

WheelJoint::~WheelJoint() {
}

void WheelJoint::SetMotorSpeed(const float & speed) {
	((b2WheelJoint*)joint)->SetMotorSpeed(speed * Physics::PIXELS_PER_METRE);
}

float WheelJoint::GetMotorSpeed() {
	return ((b2WheelJoint*)joint)->GetMotorSpeed() * Physics::METRES_PER_PIXEL;
}

void WheelJoint::Init(const std::weak_ptr<RigidBody2D>& attatchedBody, const Vector2 & anchorPoint, const Vector2 & axis, const bool & enableMotor, const float & frequency, const float & maxTorque, const float & dampingRatio) {
	b2WheelJointDef * wheelJointDef = new b2WheelJointDef();
	std::shared_ptr<RigidBody2D> wheelBody = GetComponent<RigidBody2D>().lock();
	std::shared_ptr<Transform2D> t1 = GetComponent<Transform2D>().lock(), t2 = attatchedBody.lock()->GetComponent<Transform2D>().lock();
	b2Vec2 relativeAnchorPosition = attatchedBody.lock()->GetBody()->GetWorldCenter() + TypeConversion::ConvertToB2Vector2(anchorPoint);
	Vector2 forward = (attatchedBody.lock()->GetForward());
	wheelJointDef->Initialize(attatchedBody.lock()->GetBody(), wheelBody->GetBody(), relativeAnchorPosition, b2Vec2(forward.x, forward.y));
	wheelJointDef->enableMotor = enableMotor;
	wheelJointDef->maxMotorTorque = maxTorque;
	wheelJointDef->dampingRatio = dampingRatio;
	wheelJointDef->frequencyHz = frequency;
	wheelJointDef->collideConnected = false;	
	Vector2 scale = GetComponent<Transform2D>().lock()->GetScale();
	const float halfLength = 16;
	componentData = new ComponentData();
	componentData->comp = GetWeak();
	componentData->type = Type();
	wheelJointDef->userData = componentData;
	jointDef = wheelJointDef;
	Message m = Message(MessageScope::MESSAGE_SCOPE_PHYSICS_SYSTEM, MessageType::MESSAGE_TYPE_REGISTER_JOINT, componentData);
	gameObject.lock()->HandleMessage(m);
	SetMotorSpeed(0.1f);
	SetEnabled(true);
}






