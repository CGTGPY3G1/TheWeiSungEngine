#pragma once
#ifndef WS_PLAYER_SCRIPT_H
#define WS_PLAYER_SCRIPT_H
#include "ScriptableComponent.h"
#include <memory>
class VehicleController;
class GameObject;
class PlayerScript : public ScriptableComponent {
public:
	PlayerScript();
	PlayerScript(std::weak_ptr<GameObject> gameObject);
	~PlayerScript();
	const ComponentType Type() const override { return COMPONENT_PLAYER_SCRIPT; }
	void Start() override;
	void Update(const float & deltaTime) override;
	void FixedUpdate(const float & fixedDeltaTime) override;
	const std::string GetName() const override { return "PlayerScript"; }
	int GetSortOrder() override;
	void OnCollisionEnter(const CollisionData & data) override;
	void OnCollisionExit(const CollisionData & data) override;
	void OnSensorEnter(const std::weak_ptr<Collider> & collider) override;
	void OnSensorExit(const std::weak_ptr<Collider> & collider) override;
private:
	void SetCharacterEnabled(const bool & enabled);
	void SetDriving(const bool & driving);
	bool driving = false;
	float reloadTime = 0.0f;
	std::weak_ptr<VehicleController> car;
	std::weak_ptr<GameObject> player;
};

#endif //! WS_PLAYER_SCRIPT_H