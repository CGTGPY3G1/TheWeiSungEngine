#include "RevoluteJoint.h"
#include <Box2D\Dynamics\Joints\b2RevoluteJoint.h>
#include "GameObject.h"
#include "ComponentData.h"
#include "ComponentHeaders.h"

RevoluteJoint::RevoluteJoint() : Joint() {
}

RevoluteJoint::RevoluteJoint(std::weak_ptr<GameObject> gameObject) : Joint(gameObject) {
}

RevoluteJoint::~RevoluteJoint() {
}

void RevoluteJoint::Init(const std::weak_ptr<RigidBody2D>& attatchedBody, const Vector2 & anchorPoint, const Vector2 & axis, const bool & enableMotor, const float & frequency, const float & maxTorque, const float & dampingRatio) {
	std::shared_ptr<RigidBody2D> m_Body = GetComponent<RigidBody2D>().lock();
	b2RevoluteJointDef * revoluteJointDef = new b2RevoluteJointDef();
	b2Body * otherBody = attatchedBody.lock()->GetBody(); 
	
	revoluteJointDef->Initialize(m_Body->GetBody(), otherBody, otherBody->GetWorldCenter() - TypeConversion::ConvertToB2Vector2(anchorPoint));
	//Vector2 scale = GetComponent<Transform2D>().lock()->GetScale();
	//Vector2 anchor = m_Body->GetPosition() + anchorPoint;
	//anchor -= m_Body->GetPosition();
	//b2Vec2 newAnchor =  revoluteJointDef->bodyB->GetLocalCenter();
	//revoluteJointDef->localAnchorA.Set(newAnchor.x, newAnchor.y);
	revoluteJointDef->enableLimit = true;
	revoluteJointDef->lowerAngle = 0; revoluteJointDef->upperAngle = 0;
	revoluteJointDef->enableMotor = enableMotor;
	revoluteJointDef->maxMotorTorque = maxTorque;
	revoluteJointDef->collideConnected = false;
	componentData = new ComponentData();
	componentData->comp = GetWeak();
	componentData->type = Type();
	//revoluteJointDef->userData = componentData;
	jointDef = revoluteJointDef;
	Message m = Message(MessageScope::MESSAGE_SCOPE_PHYSICS_SYSTEM, MessageType::MESSAGE_TYPE_REGISTER_JOINT, componentData);
	gameObject.lock()->HandleMessage(m);
	SetEnabled(true);
}
