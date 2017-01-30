#pragma once
#ifndef WS_CIV_WAYPOINT_SCRIPT_H
#define WS_CIV_WAYPOINT_SCRIPT_H
#include "ScriptableComponent.h"
#include "Random.h"
class Navigator {
public:
	Navigator() {}
	Navigator(const float & left, const float & top, const float & right, const float & bottom) {
		points[0][0] = Vector2(left, top);
		points[0][1] = Vector2(left, bottom);
		points[1][0] = Vector2(right, top);
		points[1][1] = Vector2(right, bottom);
	}
	~Navigator() {}

	Vector2 GetNextLocation(const Vector2 & currentLocation) {
		int x = 0, y = 0;
		float closestDistance = 1000000.0f;
		for(int i = 0; i < 2; i++) {
			for(int j = 0; j < 2; j++) {
				float distance = (points[i][j] - currentLocation).Magnitude();
				if(distance < closestDistance) {
					x = i, y = j;
					closestDistance = distance;
				}
			}
		}

		bool switchVal = Random::RandomBool();
		if(switchVal) {
			if(x == 1) x = 0;
			else x = 1;
		}
		else {
			if(y == 1) y = 0;
			else y = 1;
		}
		return points[x][y];
	}
private:
	Vector2 points[2][2];
};
class CharacterMovementScript;
class RigidBody2D;
class CivWaypointScript : public ScriptableComponent {
public:
	CivWaypointScript();
	CivWaypointScript(std::weak_ptr<GameObject> gameObject);
	~CivWaypointScript();
	const ComponentType Type() const override { return COMPONENT_CIV_WAYPOINT; }
	void Start() override;
	void FixedUpdate(const float & fixedDeltaTime) override;
	void SetTarget(const Vector2 & target);
	void SetExtents(const float & left, const float & top, const float & right, const float & bottom);
	const std::string GetName() const override { return "CivWaypointScript"; }
	template <class Archive>
	void load(Archive & ar) {
		ar(cereal::make_nvp("Target", target),
		   cereal::make_nvp("RetargetTimer", retargetTimer));
	}
	int GetSortOrder() override;
	template <class Archive>
	void save(Archive & ar) const {
		ScriptableComponent::save(ar);
		ar(cereal::make_nvp("Target", target),
		   cereal::make_nvp("RetargetTimer", retargetTimer));
	}
private:
	std::weak_ptr<RigidBody2D> myRigidBody;
	std::weak_ptr<CharacterMovementScript> movementScript;
	Vector2 target;
	Navigator navigator;
	float retargetTimer = 0.0f;
};


#endif // !WS_CIV_WAYPOINT_SCRIPT_H
