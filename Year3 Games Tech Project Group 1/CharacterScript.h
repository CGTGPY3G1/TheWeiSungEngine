#pragma once
#ifndef WS_CHARACTER_SCRIPT_H
#define WS_CHARACTER_SCRIPT_H
#include "ScriptableComponent.h"

struct AttackerInfo;

enum AIState {
	Standing = 0,
	Walking = 1,
	RunAway = 2,
	Attack = 3
};

enum AIMentality {
	Calm = 0,
	Panicking = 1,
	Angry = 2
};

class RigidBody2D;
class Transform2D;
class TileMapper;
class WeaponCache;
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
	void RunAway(const float & deltaTime);
	void Attack(const float & deltaTime);
	void NewRandomState();
	void Reset();
	const std::string GetCharacterName() const;
	const unsigned int GetCharacterID() const;
	const bool React();
	const bool HasWeapons();
	bool AvoidObstacles(const float & delta, const float & rayLengthInMetres);
	const std::string GetName() const override { return "CharacterScript"; }
	int GetSortOrder() override;
	void SetGunHandTransform(const std::shared_ptr<Transform2D> hand = std::shared_ptr<Transform2D>());
	void OnSensorEnter(const std::weak_ptr<Collider> & collider) override;
	void OnSensorExit(const std::weak_ptr<Collider> & collider) override;
	void OnCollisionEnter(const CollisionData & data) override {}
	void OnCollisionStay(const CollisionData & data) override {}
	void OnCollisionExit(const CollisionData & data) override {}
private:
	void ResetAnim();
	float GetForceScale(const Vector2 & worldPosition);
	std::weak_ptr<RigidBody2D> rigidbody;
	std::weak_ptr<Transform2D> transform, gunHandTransform;	
	std::weak_ptr<TileMapper> tileMapper;
	std::weak_ptr<WeaponCache> weapons;
	std::string characterName = "";
	unsigned int characterID = 0;
	float reactionTime = 0.0f;
	int raycastMask = (CollisionCategory::CATEGORY_ALL & ~CollisionCategory::CATEGORY_AI_CHARACTER);
	Vector2 targetLocation;
	bool isAI = false, moving = false;
	float timeUntilSwitch = 1.0f;

	AIState aiState = AIState::Standing;
	AIMentality aiMentality = AIMentality::Calm;
	void SetCurrentHostile();
	AttackerInfo GetCurrentHostile();
	unsigned int currentHostileIndex = 0;
	std::vector<AttackerInfo> hostiles;
};


#endif // !WS_CHARACTER_SCRIPT_H
