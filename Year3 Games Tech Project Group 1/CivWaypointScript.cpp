#include "CivWaypointScript.h"
#include "RigidBody2D.h"
#include "GameObject.h"
#include "CharacterMovementScript.h"
CivWaypointScript::CivWaypointScript() : ScriptableComponent() {
}

CivWaypointScript::CivWaypointScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

CivWaypointScript::~CivWaypointScript() {
}

void CivWaypointScript::Start() {
	myRigidBody = GetComponent<RigidBody2D>();
	movementScript = GetComponent<CharacterMovementScript>();
}

void CivWaypointScript::FixedUpdate(const float & fixedDeltaTime) {
	
	if(!myRigidBody.expired()) {
		std::shared_ptr<RigidBody2D> rigidBody = myRigidBody.lock();
		Vector2 forward = rigidBody->GetForward();
		Vector2 position = rigidBody->GetPosition();
		const float forwardAngle = forward.AngleInDegrees();
		Vector2 direction = (target - position).Normalized();
		rigidBody->SetRotation(forwardAngle + (forward.AngleToPointInDegrees(direction) * 0.1f));

		movementScript.lock()->MoveUsingPhysics(direction * 2.0f);
		retargetTimer -= fixedDeltaTime;
		if(retargetTimer <= 0.0f) {
			if((target - position).Magnitude() < 64.0f) {
				SetTarget(navigator.GetNextLocation(position));
				retargetTimer = 4.0f;
			}
		}
	}	
}

void CivWaypointScript::SetTarget(const Vector2 & target) {
	const float drift = 8.0f;
	const float offsetX = Random::RandomFloat(0.0f, drift * 2.0f) - drift;
	const float offsetY = Random::RandomFloat(0.0f, drift * 2.0f) - drift;
	this->target = Vector2(target.x + offsetX, target.y + offsetY);
}

void CivWaypointScript::SetExtents(const float & left, const float & top, const float & right, const float & bottom) {
	navigator = Navigator(left, top, right, bottom);
}

int CivWaypointScript::GetSortOrder() {
	return TypeInfo::ScriptSortOrder<CivWaypointScript>();
}
