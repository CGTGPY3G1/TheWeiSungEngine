#include "GrenadeScript.h"
#include "GameObjectFactory.h"
#include "PhysicsSystem.h"
#include "Transform2D.h"
#include "GameObject.h"
#include "HealthScript.h"
namespace WeiSungEngine {
	GrenadeScript::GrenadeScript() {
	}

	GrenadeScript::GrenadeScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
	}

	GrenadeScript::~GrenadeScript() {
	}

	void GrenadeScript::Detonate() {
		transform = GetComponent<Transform2D>().lock();
		const Vector2 position = transform.lock()->GetPosition();
		const float r = radius * Physics::PIXELS_PER_METRE;
		std::shared_ptr<AttackerIdentityScript> ais = GetComponent<AttackerIdentityScript>().lock();
		std::vector<std::weak_ptr<Collider>> colliders = PhysicsSystem::GetInstance().CircleCast(position, r);
		for (std::vector<std::weak_ptr<Collider>>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
			std::shared_ptr<Collider> c = (*it).lock();
			if (c) {
				if (c->GetGameObjectID() == GetGameObjectID()) continue;
				std::shared_ptr<RigidBody2D> rb = c->GetComponent<RigidBody2D>().lock();
				if (rb) {
					const Vector2 cPosition = rb->GetPosition();
					const Vector2 displacement = (cPosition - position);
					const float distance = displacement.Magnitude();
					const Vector2 direction = displacement.Normalized();
					const float distanceScale = 1.0f - (distance / r);
					const Vector2 force = direction * distanceScale * explosionForce;
					rb->AddForce(force, ForceType::IMPULSE_FORCE);
					std::shared_ptr<HealthScript> healthScript = c->GetComponent<HealthScript>().lock();
					if (healthScript) {
						healthScript->ExplosionHit(force, damage * distanceScale, 1.0f + distanceScale, ais->GetInfo());
					}
				}
			}
		}
		GameObjectFactory::CreateExplosionAnim(transform.lock()->GetPosition(), transform.lock()->GetRotation());
		gameObject.lock()->Destroy();
		alive = false;
	}

	void GrenadeScript::Update(const float & deltaTime) {
		if (lifeTime > 0.0f) lifeTime -= deltaTime;
		else if (alive) Detonate();
	}

	int GrenadeScript::GetSortOrder() {
		return 0;
	}

	void GrenadeScript::SetTimer(const float & time) {
		lifeTime = time;
	}

	const float GrenadeScript::GetBlastRadius() const {
		return radius;
	}

	void GrenadeScript::SetBlastRadius(const float & radius) {
		this->radius = radius;
	}

	const float GrenadeScript::GetDamage() const {
		return damage;
	}

	void GrenadeScript::SetDamage(const float & damage) {
		this->damage = damage;
	}

	void GrenadeScript::OnCollisionEnter(const CollisionData & data) {
		if (data.gameObject.use_count() > 0) {
			std::shared_ptr<GameObject> object = data.gameObject.lock();
			if (object->GetName().compare("Bullet") == 0) {
				std::shared_ptr<AttackerIdentityScript> newAttacker = object->GetComponent<AttackerIdentityScript>().lock();
				std::shared_ptr<AttackerIdentityScript> ais = GetComponent<AttackerIdentityScript>().lock();
				if (newAttacker && ais) {
					ais->SetAttackerName(newAttacker->GetAttackerName());
					ais->SetAttackerID(newAttacker->GetAttackerID());
				}
				Detonate();
			}
		}
	}
}
