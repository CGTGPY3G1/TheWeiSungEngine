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
	raycastMask = (CollisionCategory::CATEGORY_ALL & ~CollisionCategory::CATEGORY_AI_CHARACTER);
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
			const Vector2 right = rb->GetRight();
			const Vector2 characterPosition = rb->GetPosition();
			Vector2 rayPosition = characterPosition;
			const Vector2 offset = (right * Physics::PIXELS_PER_METRE / 5);
			const float fov = 10;
			float angle = 0;
			RayCastHit hit = PhysicsSystem::GetInstance().RayCast(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * 1.6f, raycastMask);
			angle += AngleToTurn(hit, right, characterPosition);
			rayPosition += offset;
			forward.RotateInDegrees(fov);
			hit = PhysicsSystem::GetInstance().RayCast(rayPosition, rayPosition + forward* Physics::PIXELS_PER_METRE * 0.8f, raycastMask);			
			if(hit.hit) angle -= 0.25f;
			rayPosition = characterPosition - offset;
			forward.RotateInDegrees(fov * -2.0f);
			hit = PhysicsSystem::GetInstance().RayCast(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * 0.8f, raycastMask);
			if(hit.hit) angle += 0.25f;
			if(angle < -0.000001f || angle > 0.000001f) {
				rb->AddTorque(angle * 5.0f * rb->GetMass(), ForceType::FORCE);
			}
			MoveUsingPhysics((Vector2(rb->GetMass(), 0.0f)), false);
		}
	}
}

void CharacterScript::Render() {
	if(isAI) {
		std::shared_ptr<RigidBody2D> rb = rigidbody.lock();
		if(rb) {
			Vector2 forward = rb->GetForward();
			const Vector2 right = rb->GetRight();
			const Vector2 characterPosition = rb->GetPosition();
			Vector2 rayPosition = characterPosition;
			const Vector2 offset = (right * Physics::PIXELS_PER_METRE / 5);
			float fov = 10.0f;
			std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
			graphics->DrawLine(rayPosition, rayPosition + forward * Physics::PIXELS_PER_METRE * 1.6);
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

void CharacterScript::SetArtificiallyIntelligent(const bool & isAI) {
	this->isAI = isAI;
}



int CharacterScript::GetSortOrder() {
	const static int order = TypeInfo::ScriptSortOrder<CharacterScript>();
	return order;
}
