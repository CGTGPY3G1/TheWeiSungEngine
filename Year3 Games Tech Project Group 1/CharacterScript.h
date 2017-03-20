#pragma once
#ifndef WS_CHARACTER_SCRIPT_H
#define WS_CHARACTER_SCRIPT_H
#include "ScriptableComponent.h"

enum AIState {
	Standing = 0,
	Walking = 1,
};

class RigidBody2D;
class Transform2D;
class TileMapper;
struct RayCastHit;
class CharacterScript : public ScriptableComponent{
	friend class PlayerController;
public:
	CharacterScript();
	CharacterScript(std::weak_ptr<GameObject> gameObject);
	~CharacterScript();
	const ComponentType Type() const override { return COMPONENT_CHARACTER_SCRIPT; }
	void Start() override;
	void FixedUpdate(const float & fixedDeltaTime) override;
	void Render() override;
	void MoveUsingPhysics(Vector2 & force, const bool & worldSpace = true);
	void Move(Vector2 & amount, const bool & worldSpace = true);
	float AngleToTurn(const RayCastHit & hit, Vector2 right, Vector2 position);
	const bool IsArtificiallyIntelligent() const;
	void TryToFire();
	void TryToSwitchWeapon(const bool & forward);
	void SetArtificiallyIntelligent(const bool & isAI);
	void Stand();
	void Walk(const float & deltaTime);
	void NewRandomState();
	void Reset();
	const std::string GetName() const override { return "CharacterScript"; }
	int GetSortOrder() override;
	void SetGunHandTransform(const std::shared_ptr<Transform2D> hand);
	void OnSensorEnter(const std::weak_ptr<Collider> & collider) override;
	void OnSensorExit(const std::weak_ptr<Collider> & collider) override;
private:
	void ResetAnim();
	float GetForceScale(const Vector2 & worldPosition);
	std::weak_ptr<RigidBody2D> rigidbody;
	std::weak_ptr<Transform2D> transform, gunHandTransform;	
	std::weak_ptr<TileMapper> tileMapper;
	Vector2 targetLocation;
	bool isAI = false, moving = false;
	AIState aiState = AIState::Standing;
	float timeUntilSwitch = 1.0f;
	int raycastMask = (CollisionCategory::CATEGORY_ALL & ~CollisionCategory::CATEGORY_AI_CHARACTER);
};


#endif // !WS_CHARACTER_SCRIPT_H
