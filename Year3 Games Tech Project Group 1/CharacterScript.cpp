#include "CharacterScript.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "PhysicsSystem.h"
#include "Math.h"
#include <limits>
#include "Engine.h"
#include "Graphics.h"
CharacterScript::CharacterScript() {
}

CharacterScript::CharacterScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
	raycastMask = (CollisionCategory::CATEGORY_ALL & ~CollisionCategory::CATEGORY_AI_CHARACTER);	
}

CharacterScript::~CharacterScript() {

}

void CharacterScript::Start() {
	rigidbody = GetComponent<RigidBody2D>();
	transform = GetComponent<Transform2D>();
	
		std::shared_ptr<GameObject> tm = GameObjectManager::GetInstance().GetGameObject("Tileset").lock();
		if(tm) {
			std::shared_ptr<TileMapper> map = tm->GetComponent<TileMapper>().lock();
			if(map) {
				tileMapper = tm->GetComponent<TileMapper>();
				targetLocation = map->GetNewTargetLocation(transform.lock()->GetPosition());
				NewRandomState();
			}
	}

	ResetAnim();
}

void CharacterScript::FixedUpdate(const float & fixedDeltaTime) {
	if(isAI) {
		timeUntilSwitch -= fixedDeltaTime;
		if(timeUntilSwitch <= 0.0f) NewRandomState();
		switch(aiState) {
		case AIState::Standing:
			Stand();
			break;
		case AIState::Walking:
			Walk(fixedDeltaTime);
			break;
		default:
			break;
		}
	}
	ResetAnim();
	if(moving) moving = false;
}

void CharacterScript::Render() {
	if(isAI) {
		std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
		if(rb) {
			Vector2 forward = rb->GetForward();
			const Vector2 right = rb->GetRight();
			const Vector2 characterPosition = rb->GetPosition();
			Vector2 rayPosition = characterPosition;
			const Vector2 offset = (right * Physics::PIXELS_PER_METRE * 0.2f);
			float fov = 10.0f;
			std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
			graphics->DrawLine(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * 1.6f);
			rayPosition += offset;
			forward.RotateInDegrees(fov);
			graphics->DrawLine(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * 0.8f);
			rayPosition = characterPosition - offset;
			forward.RotateInDegrees(-fov * 2.0f);
			graphics->DrawLine(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * 0.8f);
		}
	}
}

void CharacterScript::MoveUsingPhysics(Vector2 & force, const bool & worldSpace) {
	std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
	if(rb) {
		if(!moving) moving = true;
		const float tileForceScale = GetForceScale(rb->GetPosition());
		if(worldSpace) {
			const float scale = 1.0f + std::max<float>(transform.lock()->GetForward().Dot(force.Normalized()), 0.25f);
			rb->AddForce(force * tileForceScale * scale);
		}
		else {
			const float magnitude = force.Magnitude();
			Vector2 direction = transform.lock()->TransformToLocalDirection(force);
			const float scale = 1.0f + std::max<float>(transform.lock()->GetForward().Dot(direction), 0.25f);
			rb->AddForce(direction * magnitude * tileForceScale * scale);
		}
	}
}

void CharacterScript::Move(Vector2 & amount, const bool & worldSpace) {
	std::shared_ptr<Transform2D> t = transform.lock();
	if(t) {
		if(!moving) moving = true;
		const float tileForceScale = GetForceScale(t->GetPosition());
		if(worldSpace) {
			const float scale = 1.0f + std::max<float>(transform.lock()->GetForward().Dot(amount.Normalized()), 0.4f);
			t->Move(amount * tileForceScale * scale);
		}
		else {
			const float magnitude = amount.Magnitude();
			Vector2 direction = transform.lock()->TransformToLocalDirection(amount);
			const float scale = 1.0f + std::max<float>(transform.lock()->GetForward().Dot(direction), 0.4f);
			t->Move(direction * magnitude * tileForceScale * scale);
		}
	}
}

float CharacterScript::AngleToTurn(const RayCastHit & hit, Vector2 right, Vector2 position) {
	if(hit.hit) {
		float distance = std::numeric_limits<float>::max();
		size_t index = 0;
		const float dot = right.Dot(hit.normal);
		return (dot < 0.0f ? -1.0f : 1.0f) - dot;
	}	
	return 0.0f;
}

const bool CharacterScript::IsArtificiallyIntelligent() const {
	return isAI;
}

void CharacterScript::TryToFire() {
	if(gunHandTransform.use_count() == 0) SetGunHandTransform(std::weak_ptr<Transform2D>().lock());
	std::shared_ptr<Transform2D> t = gunHandTransform.lock();
	if(t) {
		std::shared_ptr<WeaponCache> w = t->GetComponent<WeaponCache>().lock();
		if(w) w->Fire();
	}
}

void CharacterScript::TryToSwitchWeapon(const bool & forward) {
	if(gunHandTransform.use_count() == 0) SetGunHandTransform(std::weak_ptr<Transform2D>().lock());
	std::shared_ptr<Transform2D> t = gunHandTransform.lock();
	if(t) {
		std::shared_ptr<WeaponCache> w = t->GetComponent<WeaponCache>().lock();
		if(w) w->SwitchWeapon(forward);	
	}
	
}

void CharacterScript::SetArtificiallyIntelligent(const bool & isAI) {
	this->isAI = isAI;
	if(isAI) NewRandomState();
}

void CharacterScript::Stand() {
	
}

void CharacterScript::Walk(const float & deltaTime) {
	std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
	if(rb) {
		Vector2 forward = rb->GetForward();
		const Vector2 right = rb->GetRight();
		const Vector2 characterPosition = rb->GetPosition();
		Vector2 rayPosition = characterPosition;
		const Vector2 offset = (right * Physics::PIXELS_PER_METRE * 0.2f);
		const float fov = 10.0f;
		float angle = 0.0f;
		bool obstacleDetected = false;
		RayCastHit hit = PhysicsSystem::GetInstance().RayCast(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * 0.8f, false);
		if(hit.hit) {
			if(!obstacleDetected) obstacleDetected = true;
			angle += AngleToTurn(hit, right, characterPosition);
		}
		
		rayPosition += offset;
		forward.RotateInDegrees(fov);
		hit = PhysicsSystem::GetInstance().RayCast(rayPosition, rayPosition + forward* Physics::PIXELS_PER_METRE * 0.5f, false);
		if(hit.hit) {
			if(!obstacleDetected) obstacleDetected = true;
			angle -= 0.25f;
		}
		rayPosition = characterPosition - offset;
		forward.RotateInDegrees(fov * -2.0f);
		hit = PhysicsSystem::GetInstance().RayCast(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * 0.5f, false);
		if(hit.hit) {
			if(!obstacleDetected) obstacleDetected = true;
			angle += 0.25f;
		}
		if(!obstacleDetected) {
			Vector2 forward = rb->GetForward();
			Vector2 position = rb->GetPosition();
			if((targetLocation - position).SquareMagnitude() < ((0.5f * Physics::PIXELS_PER_METRE) * (0.5f * Physics::PIXELS_PER_METRE)))
				targetLocation = tileMapper.lock()->GetNewTargetLocation(position);

			Vector2 direction = (targetLocation - position).Normalized();
			rb->AddTorque(forward.AngleToPointInRadians(direction) * 10.0f * deltaTime * rb->GetMass(), ForceType::FORCE);
		}
		else if(angle < -0.000001f || angle > 0.000001f) {
			rb->AddTorque(angle * 5.0f * rb->GetMass(), ForceType::FORCE);
		}
		MoveUsingPhysics((Vector2(rb->GetMass(), 0.0f)), false);
	}
}

void CharacterScript::NewRandomState() {
	int val = Random::RandomInt(100);
	if(val < 20) {
		timeUntilSwitch = Random::RandomFloat(3.5f, 10.0f);
		aiState = AIState::Standing;	
	}
	else {
		timeUntilSwitch = Random::RandomFloat(10.0f, 40.0f);
		aiState = AIState::Walking;
		std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
		if(rb) targetLocation = tileMapper.lock()->GetNewTargetLocation(rb->GetPosition());	
	}
	ResetAnim();
}

void CharacterScript::Reset() {
	NewRandomState();
}



int CharacterScript::GetSortOrder() {
	const static int order = TypeInfo::ScriptSortOrder<CharacterScript>();
	return order;
}

void CharacterScript::SetGunHandTransform(const std::shared_ptr<Transform2D> hand) {
	if(hand) {
		gunHandTransform = hand;
	}
	else {
		std::shared_ptr<Transform2D> t = transform.lock();
		bool found = false;
		for(size_t i = 0; i < t->GetChildCount() && !found; i++) {
			std::shared_ptr<Transform2D> c = t->GetChild(i).lock();
			if(c) {
				std::shared_ptr<WeaponCache> w = c->GetComponent<WeaponCache>().lock();
				if(w) {
					gunHandTransform = c;
					found = true;
				}
			}
		}
	}
}

void CharacterScript::OnSensorEnter(const std::weak_ptr<Collider>& collider) {
	
}

void CharacterScript::OnSensorExit(const std::weak_ptr<Collider>& collider) {

}

void CharacterScript::ResetAnim() {
	std::shared_ptr<Transform2D> t = gunHandTransform.lock();
	bool isArmed = false;
	if(t) {
		std::shared_ptr<WeaponCache> w = t->GetComponent<WeaponCache>().lock();
		if(w) isArmed = w->IsArmed();
	}
	if(moving) {
		std::shared_ptr<SpriteAnimator> sa = GetComponent<SpriteAnimator>().lock();
		if(sa) {
			if(!isArmed) {
				sa->PlayAnimation("Walk");

			}
			else {
				sa->PlayAnimation("WalkWithGun");
			}
		}
	}
	else {
		std::shared_ptr<SpriteAnimator> sa = GetComponent<SpriteAnimator>().lock();
		if(sa) sa->PlayAnimation(isArmed ? "IdleWithGun" : "Idle");
	}
}

float CharacterScript::GetForceScale(const Vector2 & worldPosition) {
	float tileForceScale = 1.0f;
	if(!tileMapper.expired()) tileForceScale = tileMapper.lock()->GetTileForceScale(worldPosition);
	else {
		std::shared_ptr<GameObject> tm = GameObjectManager::GetInstance().GetGameObject("Tileset").lock();
		if(tm) tileMapper = tm->GetComponent<TileMapper>();
	}
	return tileForceScale;
}
