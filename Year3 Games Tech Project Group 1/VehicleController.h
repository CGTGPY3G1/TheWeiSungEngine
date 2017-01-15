#pragma once
#ifndef WS_VEHICLE_CONTROLLER_SCRIPT_H
#define WS_VEHICLE_CONTROLLER_SCRIPT_H
#include "ScriptableComponent.h"
#include "cereal\cereal.hpp"
#include "cereal\access.hpp"
#include "cereal\details\traits.hpp"
#include <cereal\types\polymorphic.hpp>
class RigidBody2D;
class Transform2D;
class VehicleController : public ScriptableComponent {
public:
	VehicleController();
	VehicleController(std::weak_ptr<GameObject> gameObject);
	~VehicleController();
	const ComponentType Type() const override { return COMPONENT_VEHICLE_CONTROLLER; }
	void Start() override;
	void FixedUpdate(const float & fixedDeltaTime) override;
	void Drive(const float & force);
	/// <summary>
	/// Steers towards the specified direction (-1 = left | 1 = right).
	/// </summary>
	/// <param name="steerValue">A value between -1 and 1 indicating the steering angle.</param>
	void Steer(const float & steerValue = 0.0f);
	const std::string GetName() const override { return "VehicleController"; }
	//float GetMaxSteeringAngle
	template <class Archive>
	void load(Archive & ar) {

	}

	template <class Archive>
	void save(Archive & ar) const {
		ScriptableComponent::save(ar);
	}
private:
	Vector2 GetLateralVelocity(std::shared_ptr<RigidBody2D> r);
	std::weak_ptr<RigidBody2D> rigidbody;
	std::weak_ptr<Transform2D> myTransform;
};

#endif // !WS_VEHICLE_CONTROLLER_SCRIPT_H