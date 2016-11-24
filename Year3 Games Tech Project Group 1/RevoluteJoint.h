#pragma once
#ifndef WS_REVOLUTE_JOINT_H
#define WS_REVOLUTE_JOINT_H

#include "Joint.h"
class RigidBody2D;
class RevoluteJoint : public Joint {
public:
	RevoluteJoint();
	RevoluteJoint(std::weak_ptr<GameObject> gameObject);
	~RevoluteJoint();
	const ComponentType Type() const override { return COMPONENT_REVOLUTE_JOINT; }
	void Init(const std::weak_ptr<RigidBody2D> & attatchedBody, const Vector2 & anchorPoint, const Vector2 & axis, const bool & enableMotor = true, const float & frequency = 4.0f, const float & maxTorque = 100.0f, const float & dampingRatio = 0.7f);
private:

};


#endif // !WS_REVOLUTE_JOINT_H
