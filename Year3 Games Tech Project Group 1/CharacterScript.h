#pragma once
#ifndef WS_CHARACTER_SCRIPT_H
#define WS_CHARACTER_SCRIPT_H
#include "ScriptableComponent.h"

#include "cereal\cereal.hpp"
#include "cereal\access.hpp"
#include "cereal\details\traits.hpp"
#include <cereal\types\polymorphic.hpp>
class RigidBody2D;
class Transform2D;
struct RayCastHit;
class CharacterScript : public ScriptableComponent{
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
	void SetArtificiallyIntelligent(const bool & isAI);
	

	const std::string GetName() const override { return "CharacterScript"; }
	int GetSortOrder() override;
	template <class Archive>
	void load(Archive & ar) {

	}

	template <class Archive>
	void save(Archive & ar) const {
		ScriptableComponent::save(ar);
	}
private:
	
	std::weak_ptr<RigidBody2D> rigidbody;
	std::weak_ptr<Transform2D> transform;
	bool isAI = false;
	int raycastMask = ~(CollisionCategory::CATEGORY_ALL & CollisionCategory::CATEGORY_AI_CHARACTER);
};


#endif // !WS_CHARACTER_SCRIPT_H
