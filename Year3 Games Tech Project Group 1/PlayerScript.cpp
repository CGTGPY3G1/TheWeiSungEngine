#include "PlayerScript.h"
#include "Engine.h"
#include "Input.h"

#include "Transform2D.h"
#include "GameObjectFactory.h"
#include "PhysicsSystem.h"
#include "SpriteRenderer.h"
PlayerScript::PlayerScript() : ScriptableComponent() {
}

PlayerScript::PlayerScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {

}

PlayerScript::~PlayerScript() {
}

void PlayerScript::Start() {
	player = GameObjectManager::GetInstance().GetGameObject("Player");
}

void PlayerScript::Update(const float & deltaTime) {
	if(player.use_count() < 1) player = GameObjectManager::GetInstance().GetGameObject("Player");
	std::shared_ptr<Input> input = Engine::GetInstance().GetInput().lock();
	if(!driving) {
		std::shared_ptr<Transform2D> t = GetComponent<Transform2D>().lock();
		if(reloadTime > 0.0f) {
			reloadTime -= deltaTime;
		}
		if(reloadTime <= 0.0f && input->GetMouseButton(MouseButtons::Left)) {
			const float recoil = Random::RandomFloat(-10.0f, 10.0f);
			GameObjectFactory::CreateBullet(t->GetPosition() + (t->GetForward() * 64.0f).RotatedInDegrees(recoil), Vector2::One, t->GetRotation() + recoil, 20.0f * Physics::PIXELS_PER_METRE, "PlayerBullet");
			reloadTime = 0.085f;
		}
		if(input->GetKeyDown(KeyCodes::KeyCode::E) || input->GetKeyDown(KeyCodes::KeyCode::Space) || input->GetControllerButtonDown(0, ControllerButtons::ControllerButton::Y)) {
			RayCastHit hit = PhysicsSystem::GetInstance().RayCast(t->GetPosition(), t->GetPosition() + t->GetForward() * Physics::PIXELS_PER_METRE);

			if(hit.hits > 0) {
				for(size_t i = 0; i < hit.colliders.size(); i++) {
					std::shared_ptr<Collider> c = hit.colliders[i].lock();
					if(c) {
						if(c->GetTag().compare("Vehicle") == 0) {

							car = c->GetComponent<VehicleController>();
							if(car.use_count() > 0) {
								driving = true;
								std::shared_ptr<GameObject> p = player.lock();
								if(p) {
									p->GetComponent<RigidBody2D>().lock()->SetEnabled(false);
									p->GetComponent<SpriteRenderer>().lock()->SetEnabled(false);
								}
							}
						}
					}
				}
			}
		}
	}
	else {
		if(input->GetKeyDown(KeyCodes::KeyCode::E) || input->GetKeyDown(KeyCodes::KeyCode::Space) || input->GetControllerButtonDown(0, ControllerButtons::ControllerButton::Y)) {
			std::shared_ptr<VehicleController> c = car.lock();
			if(c) {
				std::shared_ptr<Transform2D> carTransform = c->GetComponent<Transform2D>().lock();
				driving = false;
				std::shared_ptr<GameObject> p = player.lock();
				if(p) {
					std::shared_ptr<RigidBody2D> rb = p->GetComponent<RigidBody2D>().lock();
					rb->SetPosition(carTransform->GetPosition() - (carTransform->GetRight() * Physics::PIXELS_PER_METRE) + (carTransform->GetForward() * Physics::PIXELS_PER_METRE));
					rb->SetVelocity(Vector2::Zero);
					rb->SetEnabled(true);
					p->GetComponent<SpriteRenderer>().lock()->SetEnabled(true);
				}
			}
		}
	}
	if(input->GetKeyDown(KeyCodes::KeyCode::P)) {
		std::shared_ptr<GameObject> p = player.lock();
		if(p) {
			std::shared_ptr<CharacterScript> cc = p->GetComponent<CharacterScript>().lock();
			if(cc) cc->SetArtificiallyIntelligent(!cc->IsArtificiallyIntelligent());
		}

	}
}

void PlayerScript::FixedUpdate(const float & fixedDeltaTime) {
	Engine engine = Engine::GetInstance();
	std::shared_ptr<Input> input = engine.GetInput().lock();
	std::shared_ptr<GameObject> gameObject;
	std::shared_ptr<GameObject> p = player.lock();
	if(p) {
		std::shared_ptr<CharacterScript> cc = p->GetComponent<CharacterScript>().lock();

		if(driving) {
			std::shared_ptr<VehicleController> c = car.lock();
			if(c) gameObject = c->GetGameObject().lock();
		}
		else {
			if(player.use_count() < 1) player = GameObjectManager::GetInstance().GetGameObject("Player");
			gameObject = player.lock();
		}
		if(gameObject) {
			std::shared_ptr<Transform2D> playerTransform = gameObject->GetComponent<Transform2D>().lock();
			bool useController = false; // input->GetControllerActive(0);

			std::shared_ptr<RigidBody2D> rb = gameObject->GetComponent<RigidBody2D>().lock();
			float dot = rb->GetVelocity().Dot(rb->GetForward());
			dot = (dot < 0.0001f) ? -1.0f : 1.0f;
			const float cameraScale = (dot < 0.0f) ? -0.4f : dot;
			const Vector2 camPos = rb->GetPosition() - engine.GetGraphics().lock()->GetCameraPosition();
			engine.GetGraphics().lock()->MoveCamera((driving ? (camPos + (rb->GetForward() * cameraScale * rb->GetSpeed())) : camPos) * (fixedDeltaTime * 1.5f));
			const float maxVelocity = Physics::PIXELS_PER_METRE * 3.0f;
			const float speed = gameObject->GetComponent<RigidBody2D>().lock()->GetSpeed();
			float oldZoom = engine.GetGraphics().lock()->GetCameraZoom();
			float newZoom = (speed / maxVelocity) * 0.4f;
			engine.GetGraphics().lock()->SetCameraZoom(std::max<float>(1.0f, (oldZoom * (1.0f - fixedDeltaTime) + ((1.0f + newZoom) * fixedDeltaTime))));
			if(cc) {
				if(!cc->IsArtificiallyIntelligent()) {
					if(driving) {
						std::shared_ptr<VehicleController> vehicle = car.lock();
						if(vehicle) {
							if(useController) {
								float force = 200.0f * input->GetAxis(0, ControllerButtons::ControllerAxis::RT);
								force -= 40.0f * input->GetAxis(0, ControllerButtons::ControllerAxis::LT);
								vehicle->Drive(force);
								vehicle->Steer(input->GetAxis(0, ControllerButtons::ControllerAxis::LeftStickH));
							}
							else {
								if(input->GetKey(KeyCodes::KeyCode::W) || input->GetKey(KeyCodes::KeyCode::Up)) vehicle->Drive(250.0f);
								else if(input->GetKey(KeyCodes::KeyCode::S) || input->GetKey(KeyCodes::KeyCode::Down)) vehicle->Drive(-50.0f);

								const float steerValue = 1.0f;
								if(input->GetKey(KeyCodes::KeyCode::A) || input->GetKey(KeyCodes::KeyCode::Left)) vehicle->Steer(-steerValue);
								else if(input->GetKey(KeyCodes::KeyCode::D) || input->GetKey(KeyCodes::KeyCode::Right)) vehicle->Steer(steerValue);
							}
						}
					}
					else {
						Vector2 mousePosition = input->GetMousePosition();
						std::shared_ptr<RigidBody2D> rb = gameObject->GetComponent<RigidBody2D>().lock();
						if(rb) {
							Vector2 forward = rb->GetForward();
							const float forwardAngle = forward.AngleInDegrees();
							float moveAmount = rb->GetMass() * 2.0f;
							if(input->GetKey(KeyCodes::KeyCode::LShift) || (useController && input->GetControllerButton(0, ControllerButtons::ControllerButton::LS))) moveAmount *= 1.5f;
							const float threshold = 0.25f;
							if(useController) {
								Vector2 directionToMouse = Vector2(input->GetAxis(0, ControllerButtons::ControllerAxis::RightStickH), input->GetAxis(0, ControllerButtons::ControllerAxis::RightStickV));
								if(directionToMouse.SquareMagnitude() > 0.5f) gameObject->GetComponent<RigidBody2D>().lock()->SetRotation(forwardAngle + (forward.AngleToPointInDegrees(directionToMouse) * 0.1f));
								Vector2 moveDirection = Vector2(input->GetAxis(0, ControllerButtons::ControllerAxis::LeftStickH), input->GetAxis(0, ControllerButtons::ControllerAxis::LeftStickV));
								gameObject->GetComponent<RigidBody2D>().lock()->AddForce(moveDirection * moveAmount * ((std::max<float>(forward.Dot(moveDirection.Normalized()), threshold)) + 1.0f), ForceType::FORCE);
							}
							else {
								Vector2 directionToMouse = (mousePosition - gameObject->GetComponent<RigidBody2D>().lock()->GetPosition());

								gameObject->GetComponent<RigidBody2D>().lock()->SetRotation(forwardAngle + (forward.AngleToPointInDegrees(directionToMouse) * 0.1f));
								if(input->GetKey(KeyCodes::KeyCode::Up) || input->GetKey(KeyCodes::KeyCode::W)) gameObject->GetComponent<CharacterScript>().lock()->MoveUsingPhysics(Vector2(0.0f, -moveAmount));
								if(input->GetKey(KeyCodes::KeyCode::Down) || input->GetKey(KeyCodes::KeyCode::S)) gameObject->GetComponent<CharacterScript>().lock()->MoveUsingPhysics(Vector2(0.0f, moveAmount));
								if(input->GetKey(KeyCodes::KeyCode::Left) || input->GetKey(KeyCodes::KeyCode::A)) gameObject->GetComponent<CharacterScript>().lock()->MoveUsingPhysics(Vector2(-moveAmount, 0.0f));
								if(input->GetKey(KeyCodes::KeyCode::Right) || input->GetKey(KeyCodes::KeyCode::D)) gameObject->GetComponent<CharacterScript>().lock()->MoveUsingPhysics(Vector2(moveAmount, 0.0f));
							}
						}
					}
				}
			}
		}
	}
}

int PlayerScript::GetSortOrder() {
	const static int order = TypeInfo::ScriptSortOrder<PlayerScript>();
	return order;
}

