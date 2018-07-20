
#ifndef WS_VEHICLE_CONTROLLER_SCRIPT_H
#define WS_VEHICLE_CONTROLLER_SCRIPT_H
#include "ScriptableComponent.h"
namespace WeiSungEngine {
	enum Wheel {
		FrontLeft = 0,
		FrontRight = 1,
		BackLeft = 2,
		BackRight = 3
	};
	class RigidBody2D;
	class TileMapper;
	class Transform2D;
	class VehicleController : public ScriptableComponent {
	public:
		VehicleController();
		VehicleController(std::weak_ptr<GameObject> gameObject);
		~VehicleController();
		const ComponentType Type() const override { return COMPONENT_VEHICLE_CONTROLLER; }
		void Start() override;
		void FixedUpdate(const float & fixedDeltaTime) override;
		void Render() override;
		void Drive(const float & force);
		void SetAccelerationForce(const float & force);
		float GetAccelerationForce();
		void SetSteeringForce(const float & force);
		float GetSteeringForce();
		void SetMaxSpeed(const float & speed);
		float GetMaxSpeed();
		/// <summary>
		/// Steers towards the specified direction (-1 = left | 1 = right).
		/// </summary>
		/// <param name="steerValue">A value between -1 and 1 indicating the steering angle.</param>
		void Steer(const float & steerValue = 0.0f);
		const std::string GetName() const override { return "VehicleController"; }
		int GetSortOrder() override;
	private:
		float GetForceScale(const Vector2 & worldPosition);
		Vector2 GetVelocityDampening(std::shared_ptr<RigidBody2D> r);
		std::weak_ptr<RigidBody2D> rigidbody;
		std::weak_ptr<Transform2D> myTransform;
		std::weak_ptr<Transform2D> wheels[4];
		Vector2 wheelsOffsets[4] = { Vector2(-50.0f, 180.0f), Vector2(50.0f, 180.0f), Vector2(-50.0f, 0), Vector2(50.0f, 0) };
		std::weak_ptr<TileMapper> tileMapper;
		float accelerationForce = 30.0f, steeringForce = 1.2f, maxSpeed = 2000.0f, squaredMaxSpeed = maxSpeed * maxSpeed;
	};
}
#endif // !WS_VEHICLE_CONTROLLER_SCRIPT_H
