#include "CharacterScript.h"
#include "GameObject.h"
#include "PhysicsSystem.h"
#include "Math.h"
#include <limits>
#include "Engine.h"
#include "Graphics.h"
CharacterScript::CharacterScript() {
}
CharacterScript::CharacterScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

CharacterScript::~CharacterScript() {
}

void CharacterScript::Start() {
	rigidbody = GetComponent<RigidBody2D>();
	transform = GetComponent<Transform2D>();
}

void CharacterScript::FixedUpdate(const float & fixedDeltaTime) {
	if(isAI) {
		std::shared_ptr<RigidBody2D> rb = rigidbody.lock();		
		if(rb) {
			Vector2 forward = rb->GetForward();
			Vector2 right = rb->GetRight();
			const Vector2 characterPosition = rb->GetPosition();
			Vector2 rayPosition = characterPosition;
			Vector2 offset = (right * Physics::PIXELS_PER_METRE / 4);
			float fov = 15.0f;
			float angle = 0;
			RayCastHit hit = PhysicsSystem::GetInstance().RayCast(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * 1.6, raycastMask);
			angle += AngleToTurn(hit, right, characterPosition);
			rayPosition += offset;
			forward.RotateInDegrees(fov);
			right.RotateInDegrees(fov);
			hit = PhysicsSystem::GetInstance().RayCast(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE, raycastMask);
			float angle2 = AngleToTurn(hit, right, characterPosition);
			if(angle2 > 0)
				angle += angle2;
			else
				angle -= angle2;
			rayPosition = characterPosition - offset;
			forward.RotateInDegrees(-fov * 2.0f);
			right.RotateInDegrees(-fov * 2.0f);
			hit = PhysicsSystem::GetInstance().RayCast(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE, raycastMask);
			angle2 = AngleToTurn(hit, right, characterPosition);
			if(angle2 > 0)
				angle += angle2;
			else
				angle -= angle2;
			if(angle < -0.0001f || angle > 0.0001f) {
				rb->AddTorque(angle*2.0f, ForceType::IMPULSE_FORCE);
			}
			else {
				const float angularVelocity = rb->GetAngularVelocity();
				if(angularVelocity < -0.0001f || angularVelocity > 0.0001f) {
					rb->AddTorque(-angularVelocity, ForceType::IMPULSE_FORCE);
				}
			}
			float moveAmount = rb->GetMass();
			MoveUsingPhysics((Vector2(moveAmount, 0.0f)), false);
		}
	}
}

void CharacterScript::Render() {
	std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
	if(rb) {
		Vector2 forward = rb->GetForward();
		Vector2 right = rb->GetRight();
		const Vector2 characterPosition = rb->GetPosition();
		Vector2 rayPosition = characterPosition;
		Vector2 offset = (right * Physics::PIXELS_PER_METRE / 4);
		float fov = 15.0f;
		float angle = 0;
		std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
		graphics->DrawLine(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * 1.6);
		rayPosition += offset;
		forward.RotateInDegrees(fov);
		right.RotateInDegrees(fov);
		graphics->DrawLine(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE);
		rayPosition = characterPosition - offset;
		forward.RotateInDegrees(-fov * 2.0f);
		right.RotateInDegrees(-fov * 2.0f);
		graphics->DrawLine(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE);
	}
}

void CharacterScript::MoveUsingPhysics(Vector2 & force, const bool & worldSpace) {
	std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
	if(rb) {
		if(worldSpace) {
			const float scale = 1.0f + std::max<float>(transform.lock()->GetForward().Dot(force.Normalized()), 0.25f);
			rb->AddForce(force * scale);
		}
		else {
			const float magnitude = force.Magnitude();
			Vector2 direction = transform.lock()->TransformToLocalDirection(force);
			const float scale = 1.0f + std::max<float>(transform.lock()->GetForward().Dot(direction), 0.25f);
			rb->AddForce(direction * magnitude * scale);
		}
	}
}

void CharacterScript::Move(Vector2 & amount, const bool & worldSpace) {
	std::shared_ptr<Transform2D> t = transform.lock();
	if(worldSpace) {
		t->Move(amount);
	}
	else {
		const float magnitude = amount.Magnitude();
		Vector2 direction = transform.lock()->TransformToLocalDirection(amount);
		const float scale = 1.0f + std::max<float>(transform.lock()->GetForward().Dot(direction), 0.4f);
		t->Move(direction * magnitude * scale);
	}
}

float CharacterScript::AngleToTurn(const RayCastHit & hit, Vector2 right, Vector2 position) {
	if(hit.hits > 0) {
		float squaredDistance = std::numeric_limits<float>::max();
		size_t index = 0;
		for(size_t i = 0; i < hit.colliders.size(); i++) {
			std::shared_ptr<Collider> c = hit.colliders[i].lock();
			if(c) {
				const float dist = (position - hit.points[i]).SquareMagnitude();
				if(dist < squaredDistance) {
					squaredDistance = dist;
					index = i;
				}			
			}
		}
		const float dot = right.Dot(hit.normals[index]);
		return dot < 0.0f ? -1.0f + dot : 1.0f - dot;
	}
	return 0.0f;
}

const bool CharacterScript::IsArtificiallyIntelligent() const {
	return isAI;
}

void CharacterScript::SetArtificiallyIntelligent(const bool & isAI) {
	this->isAI = isAI;
}

float CharacterScript::GetHealth() {
	return health;
}

const float CharacterScript::GetHealth() const {
	return health;
}

void CharacterScript::SetHealth(const float newHealth) {
	health = newHealth;
}

void CharacterScript::AddToHealth(const float amount) {
	health -= amount;
}

int CharacterScript::GetSortOrder() {
	const static int order = TypeInfo::ScriptSortOrder<CharacterScript>();
	return order;
}
