#pragma once
#ifndef WS_WHEEL_JOINT_H
#define WS_WHEEL_JOINT_H

#include "Joint.h"
class RigidBody2D;
class WheelJoint : public Joint {
public:
	friend class PhysicsSystem;
	WheelJoint();
	WheelJoint(std::weak_ptr<GameObject> gameObject);
	~WheelJoint();
	float GetMotorSpeed();
	void SetMotorSpeed(const float & speed);
	void SetTurnAngle(const float & degrees);
	float GetMaxTurnAngle();
	float GetAngle();

	const ComponentType Type() const override { return COMPONENT_WHEEL_JOINT; }
	void Init(const std::weak_ptr<RigidBody2D> & attatchedBody, const Vector2 & anchorPoint, const Vector2 & axis, const bool & enableMotor = true, const float & frequency = 4.0f, const float & maxTorque = 100.0f, const float & dampingRatio = 0.7f);
	
	template <class Archive>
	void load(Archive & ar) {

	}

	template <class Archive>
	void save(Archive & ar) const {

	}
private:
	bool steering = false;
};



#endif // !WS_WHEEL_JOINT_H+H