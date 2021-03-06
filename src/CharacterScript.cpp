#include "CharacterScript.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "PhysicsSystem.h"
#include "Math.h"
#include <limits>
#include "Engine.h"
#include "Graphics.h"
#include "NameGenerator.h"
#include "AttackerIdentityScript.h"
#include <algorithm>
namespace WeiSungEngine {
	CharacterScript::CharacterScript() {
	}

	CharacterScript::CharacterScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
		raycastMask = (CollisionCategory::CATEGORY_ALL & ~CollisionCategory::CATEGORY_AI_CHARACTER);
		if (gameObject.use_count() > 0) {
			characterName = (gameObject.lock()->GetName().compare("Player") == 0) ? "Player" : NameGenerator::GetInstance().GetRandomName();
			characterID = gameObject.lock()->GetCombinedObjectID();
		}
		else {
			characterName = NameGenerator::GetInstance().GetRandomName();
			characterID = 0;
		}
	}

	CharacterScript::~CharacterScript() {

	}

	void CharacterScript::Start() {
		rigidbody = GetComponent<RigidBody2D>();
		transform = GetComponent<Transform2D>();

		std::shared_ptr<GameObject> tm = GameObjectManager::GetInstance().GetGameObject("Tileset").lock();
		if (tm) {
			std::shared_ptr<TileMapper> map = tm->GetComponent<TileMapper>().lock();
			if (map) {
				tileMapper = tm->GetComponent<TileMapper>();
				targetLocation = map->GetNewTargetLocation(transform.lock()->GetPosition());
				NewRandomState();
			}
		}
		ResetAnim();
	}

	void CharacterScript::FixedUpdate(const float & fixedDeltaTime) {
		if (isAI) {
			switch (aiState) {
			case AIState::Standing:
				Stand();
				timeUntilSwitch -= fixedDeltaTime;
				if (timeUntilSwitch <= 0.0f) NewRandomState();
				break;
			case AIState::Walking:
				Walk(fixedDeltaTime);
				timeUntilSwitch -= fixedDeltaTime;
				if (timeUntilSwitch <= 0.0f) NewRandomState();
				break;
			case AIState::RunAway:
				RunAway(fixedDeltaTime);
				break;
			case AIState::Attack:
				Attack(fixedDeltaTime);
				break;
			default:
				break;
			}
		}
		ResetAnim();
		if (moving) moving = false;
	}

	void CharacterScript::MoveUsingPhysics(const Vector2 & force, const bool & worldSpace) {
		std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
		if (rb) {
			if (!moving) moving = true;
			const float tileForceScale = GetForceScale(rb->GetPosition());
			if (worldSpace) {
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
		if (t) {
			if (!moving) moving = true;
			const float tileForceScale = GetForceScale(t->GetPosition());
			if (worldSpace) {
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
		if (hit.hit) {
			float distance = std::numeric_limits<float>::max();
			size_t index = 0;
			const float dot = right.Dot(hit.normal);
			return dot;
		}
		return 0.0f;
	}

	const bool CharacterScript::IsArtificiallyIntelligent() const {
		return isAI;
	}

	void CharacterScript::TryToFire() {
		if (gunHandTransform.use_count() == 0) SetGunHandTransform(std::weak_ptr<Transform2D>().lock());
		std::shared_ptr<WeaponCache> w = weapons.lock();
		if (w) w->Fire();
	}

	void CharacterScript::TryToSwitchWeapon(const bool & forward) {
		if (gunHandTransform.use_count() == 0) SetGunHandTransform(std::weak_ptr<Transform2D>().lock());
		std::shared_ptr<Transform2D> t = gunHandTransform.lock();
		if (t) {
			std::shared_ptr<WeaponCache> w = t->GetComponent<WeaponCache>().lock();
			if (w) w->SwitchWeapon(forward);
		}

	}

	void CharacterScript::SetArtificiallyIntelligent(const bool & isAI) {
		this->isAI = isAI;
		if (isAI) NewRandomState();
	}

	void CharacterScript::Stand() {

	}

	void CharacterScript::Walk(const float & deltaTime) {
		std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
		if (rb) {
			if (!AvoidObstacles(deltaTime, 0.8f)) {
				Vector2 forward = rb->GetForward();
				Vector2 position = rb->GetPosition();
				if ((targetLocation - position).SquareMagnitude() < ((0.5f * Physics::PIXELS_PER_METRE) * (0.5f * Physics::PIXELS_PER_METRE)))
					targetLocation = tileMapper.lock()->GetNewTargetLocation(position);

				Vector2 direction = (targetLocation - position).Normalized();
				rb->AddTorque(forward.AngleToPointInRadians(direction) * 10.0f * deltaTime * rb->GetMass(), ForceType::FORCE);
			}
			MoveUsingPhysics((Vector2(rb->GetMass(), 0.0f)), false);
		}
	}

	void CharacterScript::RunAway(const float & deltaTime) {
		AttackerInfo currentHostile = GetCurrentHostile();
		std::shared_ptr<WeaponCache> w = weapons.lock();
		if (IsArmed()) TryToSwitchWeapon(false);
		if (currentHostile.IsAlive()) {
			std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
			if (rb) {
				if (!AvoidObstacles(deltaTime, 2.0f)) {
					Vector2 forward = rb->GetForward();
					Vector2 position = rb->GetPosition();
					Vector2 hostilePosition = currentHostile.GetPosition();
					Vector2 runDirection = (position - hostilePosition);
					rb->AddTorque(forward.AngleToPointInRadians(runDirection) * 10.0f * deltaTime * rb->GetMass(), ForceType::FORCE);
				}
				MoveUsingPhysics((Vector2(rb->GetMass() * 2.5f, 0.0f)), false);
			}
		}
		else {
			NewRandomState();
		}
	}

	void CharacterScript::Attack(const float & deltaTime) {
		AttackerInfo currentHostile = GetCurrentHostile();
		if (currentHostile.IsAlive()) {
			std::shared_ptr<WeaponCache> w = weapons.lock();
			if (w && w->HasWeapons() && !w->IsArmed()) TryToSwitchWeapon(true);
			std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
			if (rb) {
				const Vector2 characterPosition = rb->GetPosition();
				const Vector2 hostilePosition = currentHostile.GetPosition();
				const Vector2 displacement = hostilePosition - characterPosition;
				const bool inRange = displacement.SquareMagnitude() < (Physics::PIXELS_PER_METRE * Physics::PIXELS_PER_METRE * 100.0f);
				if (!inRange) {
					if (!AvoidObstacles(deltaTime, 1.5f)) {
						Vector2 forward = rb->GetForward();
						Vector2 runDirection = (hostilePosition - characterPosition);
						rb->AddTorque(forward.AngleToPointInRadians(runDirection) * 10.0f * deltaTime * rb->GetMass(), ForceType::FORCE);
					}
					MoveUsingPhysics((Vector2(rb->GetMass() * 2.5f, 0.0f)), false);
				}
				else {
					const Vector2 forward = rb->GetForward();
					Vector2 direction = displacement.Normalized();
					const float angleToTarget = forward.AngleToPointInRadians(direction);
					rb->AddTorque(forward.AngleToPointInRadians(direction) * 10.0f * deltaTime * rb->GetMass(), ForceType::FORCE);

					bool moving = false;
					Vector2 moveDirection = Vector2(0.0f, 0.0f);
					RayCastHit hit = PhysicsSystem::GetInstance().RayCast(characterPosition, characterPosition + displacement, false, CollisionCategory::CATEGORY_BUILDING);
					if (hit.hit) {
						const Vector2 right = rb->GetRight();
						moveDirection += right * right.Dot(hit.normal) * -2.5f;
						moving = true;

						hit = PhysicsSystem::GetInstance().RayCast(characterPosition, characterPosition + displacement, false, CollisionCategory::CATEGORY_BUILDING);
						if (hit.hit) {
							moveDirection += forward * -forward.Dot(hit.normal) * 2.5f;
						}
						MoveUsingPhysics(moveDirection * rb->GetMass(), false);
					}
					else if (inRange) {
						if (angleToTarget < 0.01f && angleToTarget > -0.01f && w && w->HasWeapons() && w->IsArmed()) w->Fire();
					}
				}

			}
		}
		else {
			SetClosestHostileAsCurrent();
			currentHostile = GetCurrentHostile();
			if (!currentHostile.IsAlive()) NewRandomState();
		}
	}

	void CharacterScript::NewRandomState() {
		int val = Random::RandomInt(100);
		if (val < 20) {
			timeUntilSwitch = Random::RandomFloat(3.5f, 10.0f);
			aiState = AIState::Standing;
			aiMentality = AIMentality::Calm;
			hostiles.clear();
		}
		else {
			timeUntilSwitch = Random::RandomFloat(10.0f, 40.0f);
			aiState = AIState::Walking;
			std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
			if (rb) targetLocation = tileMapper.lock()->GetNewTargetLocation(rb->GetPosition());
			aiMentality = AIMentality::Calm;
			hostiles.clear();

		}
		int count = 0;
		while (IsArmed() && count < 3) {
			TryToSwitchWeapon(true);
			count++;
		}
		ResetAnim();
	}

	void CharacterScript::Reset() {
		//NewRandomState();
	}

	const std::string CharacterScript::GetCharacterName() const {
		return characterName;
	}

	const unsigned int CharacterScript::GetCharacterID() const {
		return characterID;
	}

	const bool CharacterScript::React(const bool & nearestContact) {
		if (nearestContact) SetClosestHostileAsCurrent();
		if (gunHandTransform.use_count() == 0) SetGunHandTransform(std::weak_ptr<Transform2D>().lock());
		std::shared_ptr<Transform2D> t = gunHandTransform.lock();
		std::shared_ptr<HealthScript> hs = GetComponent<HealthScript>().lock();
		if (hs && hs->GetHealthAsPercentage() < 10.0f) {
			if (aiMentality != AIMentality::Panicking) aiMentality = AIMentality::Panicking;
			if (aiState != AIState::RunAway) aiState = AIState::RunAway;
		}
		else {
			if (t) {
				std::shared_ptr<WeaponCache> w = weapons.lock();
				if (w && w->HasWeapons()) {
					if (aiMentality != AIMentality::Angry) aiMentality = AIMentality::Angry;
					if (aiState != AIState::Attack) aiState = AIState::Attack;
				}
				else {
					if (aiMentality != AIMentality::Panicking) aiMentality = AIMentality::Panicking;
					if (aiState != AIState::RunAway) aiState = AIState::RunAway;
				}
			}
			else {
				if (aiMentality != AIMentality::Panicking) aiMentality = AIMentality::Panicking;
				if (aiState != AIState::RunAway) aiState = AIState::RunAway;
			}
		}
		if (aiMentality != AIMentality::Angry) {
			int count = 0;
			while (IsArmed() && count < 3) {
				TryToSwitchWeapon(true);
				count++;
			}
		}
		return true;
	}

	const bool CharacterScript::HasWeapons() {
		if (weapons.use_count() == 0) SetGunHandTransform();
		return weapons.use_count() > 0 && weapons.lock()->HasWeapons();
	}

	const bool CharacterScript::IsArmed() {
		if (weapons.use_count() == 0) SetGunHandTransform();
		return weapons.use_count() > 0 && weapons.lock()->IsArmed();
	}

	bool CharacterScript::AvoidObstacles(const float & delta, const float & rayLengthInMetres) {
		std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
		Vector2 forward = rb->GetForward();
		const Vector2 right = rb->GetRight();
		const Vector2 characterPosition = rb->GetPosition();
		Vector2 rayPosition = characterPosition;
		const Vector2 offset = (right * Physics::PIXELS_PER_METRE * 0.2f);
		const float fov = 10.0f;
		float angle = 0.0f;
		bool obstacleDetected = false;
		bool sideHit = false;
		RayCastHit hit = PhysicsSystem::GetInstance().RayCast(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * rayLengthInMetres, false);
		if (hit.hit) {
			if (!obstacleDetected) obstacleDetected = true;
			angle += AngleToTurn(hit, forward, characterPosition);
		}

		rayPosition += offset;
		forward.RotateInDegrees(fov);
		hit = PhysicsSystem::GetInstance().RayCast(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * rayLengthInMetres * 0.75f, false);
		if (hit.hit) {
			if (!obstacleDetected) obstacleDetected = true;
			angle -= 1.0f;
		}
		rayPosition = characterPosition - offset;
		forward.RotateInDegrees(fov * -2.0f);
		hit = PhysicsSystem::GetInstance().RayCast(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * rayLengthInMetres * 0.75f, false);
		if (hit.hit) {
			if (!obstacleDetected) obstacleDetected = true;
			if (!sideHit) angle += 1.0f;
		}
		if (obstacleDetected) {
			if (angle < -0.000001f || angle > 0.000001f) rb->AddTorque(angle * rb->GetMass(), ForceType::FORCE);
			return true;
		}
		return false;
	}



	int CharacterScript::GetSortOrder() {
		const static int order = TypeInfo::ScriptSortOrder<CharacterScript>();
		return order;
	}

	std::weak_ptr<WeaponCache> CharacterScript::GetWeaponCache() {
		if (weapons.use_count() == 0) SetGunHandTransform();
		return weapons;
	}

	void CharacterScript::SetGunHandTransform(const std::shared_ptr<Transform2D> hand) {
		if (hand) {
			gunHandTransform = hand;
			std::shared_ptr<WeaponCache> w = hand->GetComponent<WeaponCache>().lock();
			if (w) {
				weapons = w;
				w->SetShooter(std::static_pointer_cast<CharacterScript>(shared_from_this()));
			}
		}
		else {
			std::shared_ptr<Transform2D> t = GetComponent<Transform2D>().lock();
			if (t) {
				bool found = false;
				for (size_t i = 0; i < t->GetChildCount() && !found; i++) {
					std::shared_ptr<Transform2D> c = t->GetChild(i).lock();
					if (c) {
						std::shared_ptr<WeaponCache> w = c->GetComponent<WeaponCache>().lock();
						if (w) {
							weapons = w;
							w->SetShooter(std::static_pointer_cast<CharacterScript>(shared_from_this()));
							gunHandTransform = c;
							found = true;
						}
					}
				}
			}
		}
	}

	void CharacterScript::OnSensorEnter(const std::weak_ptr<Collider>& collider) {
		if (isAI) {
			if (collider.use_count() > 0) {
				std::shared_ptr<Collider> c = collider.lock();
				std::shared_ptr<DamageScript> ds = c->GetComponent<DamageScript>().lock();
				std::shared_ptr<GrenadeScript> gs = c->GetComponent<GrenadeScript>().lock();
				if (ds || gs) {
					std::shared_ptr<AttackerIdentityScript> ais = c->GetComponent<AttackerIdentityScript>().lock();
					if (ais) {
						if (ais->IsValid()) {
							AttackerInfo info = ais->GetInfo();
							if (info.iD != characterID) {
								if (hostiles.size() == 0 || std::find(hostiles.begin(), hostiles.end(), info) == hostiles.end()) {
									hostiles.push_back(info);
									React(true);
								}
								else {
									React(true);
								}
							}
						}
					}
				}
			}
		}
		if (collider.use_count() > 0) {
			std::shared_ptr<GameObject> g = collider.lock()->GetGameObject().lock();
			if (g->GetTag().compare("Collectable") == 0) {
				std::shared_ptr<Collectable> c = g->GetComponent<Collectable>().lock();
				if (c) {
					if (c->IsValid()) {
						CollectionCache cache = c->Activate();
						switch (cache.type) {
						case CollectableType::CollectableHealth:
						{
							std::shared_ptr<HealthScript> hs = GetComponent<HealthScript>().lock();
							if (hs) hs->AddToHealth(cache.amount);
						}
						break;
						case CollectableType::CollectablePistol:
						case CollectableType::CollectableUzi:
						case CollectableType::CollectableGrenade:
						{
							std::shared_ptr<CharacterScript> cs = GetComponent<CharacterScript>().lock();
							if (cs) {
								std::shared_ptr<WeaponCache> wc = cs->GetWeaponCache().lock();
								if (wc) wc->AddAmmo(cache.type == CollectableType::CollectablePistol ? WeaponTypePistol : cache.type == CollectableType::CollectableUzi ? WeaponTypeUzi : WeaponTypeGrenade, cache.amount);
							}
						}
						break;
						default:
							break;
						}
					}
				}
			}
		}
	}

	void CharacterScript::OnSensorExit(const std::weak_ptr<Collider>& collider) {

	}

	void CharacterScript::OnCollisionEnter(const CollisionData & data) {
		if (isAI) {
			if (data.gameObject.use_count() > 0) {
				std::shared_ptr<GameObject> c = data.gameObject.lock();
				std::shared_ptr<DamageScript> ds = c->GetComponent<DamageScript>().lock();
				if (ds) {
					std::shared_ptr<AttackerIdentityScript> ais = c->GetComponent<AttackerIdentityScript>().lock();
					if (ais) {
						if (ais->IsValid()) {
							AttackerInfo info = ais->GetInfo();
							if (info.iD != characterID) {
								SetCurrentHostile(info);
								React(false);
							}
						}
					}
				}
			}
		}
	}

	void CharacterScript::ResetAnim() {
		std::shared_ptr<Transform2D> t = gunHandTransform.lock();
		bool isArmed = false;
		if (t) {
			std::shared_ptr<WeaponCache> w = t->GetComponent<WeaponCache>().lock();
			if (w) isArmed = w->IsArmed();
		}
		if (moving) {
			std::shared_ptr<SpriteAnimator> sa = GetComponent<SpriteAnimator>().lock();
			if (sa) {
				if (!isArmed) {
					sa->PlayAnimation("Walk");
				}
				else {
					sa->PlayAnimation("WalkWithGun");
				}
			}
		}
		else {
			std::shared_ptr<SpriteAnimator> sa = GetComponent<SpriteAnimator>().lock();
			if (sa) sa->PlayAnimation(isArmed ? "IdleWithGun" : "Idle");
		}
	}

	float CharacterScript::GetForceScale(const Vector2 & worldPosition) {
		float tileForceScale = 1.0f;
		if (!tileMapper.expired()) tileForceScale = tileMapper.lock()->GetTileForceScale(worldPosition);
		else {
			std::shared_ptr<GameObject> tm = GameObjectManager::GetInstance().GetGameObject("Tileset").lock();
			if (tm) tileMapper = tm->GetComponent<TileMapper>();
		}
		return tileForceScale;
	}

	void CharacterScript::SetCurrentHostile(const AttackerInfo & hostile) {
		hostiles.push_back(hostile);
		currentHostileIndex = hostiles.size() - 1;
	}

	void CharacterScript::SetClosestHostileAsCurrent() {
		std::shared_ptr<Transform2D> t = transform.lock();
		if (t) {
			const Vector2 myPosition = t->GetPosition();
			float nearestDistance = std::numeric_limits<float>().max();
			unsigned int nearestIndex = 0;
			unsigned int index = 0;
			std::vector<AttackerInfo>::iterator it = hostiles.begin();
			while (it != hostiles.end()) {
				if ((*it).IsAlive()) {
					const float sqrDistance = (myPosition - (*it).GetPosition()).SquareMagnitude();
					if (sqrDistance < nearestDistance) {
						nearestDistance = sqrDistance;
						nearestIndex = index;
					}
					++it;
					++index;
				}
				else {
					it = hostiles.erase(it);
				}
			}
		}
	}

	AttackerInfo CharacterScript::GetCurrentHostile() {
		if (currentHostileIndex >= hostiles.size()) return AttackerInfo();
		return hostiles[currentHostileIndex];
	}
}
