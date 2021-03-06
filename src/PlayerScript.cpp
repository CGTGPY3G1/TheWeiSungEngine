#include "PlayerScript.h"
#include "Engine.h"
#include "Input.h"
#include "Transform2D.h"
#include "GameObjectFactory.h"
#include "PhysicsSystem.h"
#include "SpriteRenderer.h"
#include "HealthScript.h"
#include "CharacterScript.h"
#include <SFML\Audio\Listener.hpp>
namespace WeiSungEngine {
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
		if (player.use_count() < 1) player = GameObjectManager::GetInstance().GetGameObject("Player");
		std::shared_ptr<Input> input = Engine::GetInstance().GetInput().lock();
		/*const std::shared_ptr<GameObject> tileMap = GameObjectManager::GetInstance().GetGameObject("BackgroundTileset").lock();
		if(tileMap) {
			const std::shared_ptr<TileMapper> tileMapper = tileMap->GetComponent<TileMapper>().lock();
			if(tileMapper)
				std::cout << tileMapper->GetTileTypeAsString(player.lock()->GetComponent<Transform2D>().lock()->GetPosition()) << std::endl;
		}*/

		if (!driving) {
			std::shared_ptr<Transform2D> t = GetComponent<Transform2D>().lock();
			if (reloadTime > 0.0f) {
				reloadTime -= deltaTime;
			}
			if (reloadTime <= 0.0f && input->GetMouseButton(MouseButtons::Left)) {
				std::shared_ptr<CharacterScript> cc = t->GetComponent<CharacterScript>().lock();
				if (cc) cc->TryToFire();
			}
			if (input->GetKeyDown(KeyCodes::KeyCode::Num1)) {
				std::shared_ptr<CharacterScript> cc = t->GetComponent<CharacterScript>().lock();
				if (cc) cc->TryToSwitchWeapon(false);
			}
			else if (input->GetKeyDown(KeyCodes::KeyCode::Num2)) {
				std::shared_ptr<CharacterScript> cc = t->GetComponent<CharacterScript>().lock();
				if (cc) cc->TryToSwitchWeapon(true);
			}
			if (input->GetKeyDown(KeyCodes::KeyCode::E) || input->GetKeyDown(KeyCodes::KeyCode::Space) || input->GetControllerButtonDown(0, ControllerButtons::ControllerButton::Y)) {
				RayCastHit hit = PhysicsSystem::GetInstance().RayCast(t->GetPosition(), t->GetPosition() + t->GetForward() * Physics::PIXELS_PER_METRE);

				if (hit.hit) {
					std::shared_ptr<Collider> c = hit.collider.lock();
					if (c) {
						if (c->GetTag().compare("Vehicle") == 0) {
							car = c->GetComponent<VehicleController>();
							if (car.use_count() > 0) {
								SetDriving(true);
							}
						}
					}
				}
			}
		}
		else {
			std::shared_ptr<VehicleController> c = car.lock();
			if (c) {
				std::shared_ptr<RigidBody2D> carRB = c->GetComponent<RigidBody2D>().lock();
				std::shared_ptr<GameObject> p = player.lock();
				std::shared_ptr<RigidBody2D> rb = p->GetComponent<RigidBody2D>().lock();
				if (input->GetKeyDown(KeyCodes::KeyCode::E) || input->GetControllerButtonDown(0, ControllerButtons::ControllerButton::Y)) {
					SetDriving(false);
				}
				else {
					rb->SetPosition(carRB->GetPosition());
				}
			}
			else SetDriving(false);
		}
	}

	void PlayerScript::FixedUpdate(const float & fixedDeltaTime) {
		Engine engine = Engine::GetInstance();
		std::shared_ptr<Input> input = engine.GetInput().lock();
		std::shared_ptr<GameObject> gameObject;
		std::shared_ptr<GameObject> p = player.lock();
		if (p) {
			if (driving) {
				std::shared_ptr<VehicleController> c = car.lock();
				if (c) {
					gameObject = c->GetGameObject().lock();
					if (!gameObject) SetDriving(false);
				}
				else SetDriving(false);
			}
			if (!driving) {
				if (player.use_count() < 1) player = GameObjectManager::GetInstance().GetGameObject("Player");
				gameObject = player.lock();
			}
			if (gameObject) {
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
				engine.GetGraphics().lock()->SetCameraZoom(std::max<float>(1.5f, (oldZoom * (1.0f - fixedDeltaTime) + ((1.5f + newZoom) * fixedDeltaTime))));
				std::shared_ptr<CharacterScript> cc = p->GetComponent<CharacterScript>().lock();
				const Vector2 listenerPosition = engine.GetGraphics().lock()->GetCameraPosition();
				Vector2 r = playerTransform->GetRight();
				sf::Listener::setDirection(r.x, r.y, 0.0f);
				sf::Listener::setUpVector(0.0f, 1.0f, 0.0f);
				sf::Listener::setPosition(listenerPosition.x, listenerPosition.y, 1.0f);
				if (cc) {
					if (!cc->IsArtificiallyIntelligent()) {
						if (driving) {
							std::shared_ptr<VehicleController> vehicle = car.lock();
							if (vehicle) {
								if (useController) {
									float force = 1.0f * input->GetAxis(0, ControllerButtons::ControllerAxis::RT);
									force -= 0.2f * input->GetAxis(0, ControllerButtons::ControllerAxis::LT);
									vehicle->Drive(force);
									vehicle->Steer(input->GetAxis(0, ControllerButtons::ControllerAxis::LeftStickH));
								}
								else {
									if (input->GetKey(KeyCodes::KeyCode::W) || input->GetKey(KeyCodes::KeyCode::Up)) vehicle->Drive(1.0f);
									else if (input->GetKey(KeyCodes::KeyCode::S) || input->GetKey(KeyCodes::KeyCode::Down)) vehicle->Drive(-0.2f);

									const float steerValue = 1.0f;
									if (input->GetKey(KeyCodes::KeyCode::A) || input->GetKey(KeyCodes::KeyCode::Left)) vehicle->Steer(-steerValue);
									else if (input->GetKey(KeyCodes::KeyCode::D) || input->GetKey(KeyCodes::KeyCode::Right)) vehicle->Steer(steerValue);
								}
							}
						}
						else {
							Vector2 mousePosition = input->GetMousePosition();
							std::shared_ptr<RigidBody2D> rb = gameObject->GetComponent<RigidBody2D>().lock();
							if (rb) {
								Vector2 forward = rb->GetForward();
								const float forwardAngle = forward.AngleInDegrees();
								float moveAmount = rb->GetMass() * 2.0f;
								if (input->GetKey(KeyCodes::KeyCode::LShift) || (useController && input->GetControllerButton(0, ControllerButtons::ControllerButton::LS))) moveAmount *= 1.5f;
								const float threshold = 0.25f;
								if (useController) {
									Vector2 directionToMouse = Vector2(input->GetAxis(0, ControllerButtons::ControllerAxis::RightStickH), input->GetAxis(0, ControllerButtons::ControllerAxis::RightStickV));
									if (directionToMouse.SquareMagnitude() > 0.5f) gameObject->GetComponent<RigidBody2D>().lock()->SetRotation(forwardAngle + (forward.AngleToPointInDegrees(directionToMouse) * 0.1f));
									Vector2 moveDirection = Vector2(input->GetAxis(0, ControllerButtons::ControllerAxis::LeftStickH), input->GetAxis(0, ControllerButtons::ControllerAxis::LeftStickV));
									gameObject->GetComponent<RigidBody2D>().lock()->AddForce(moveDirection * moveAmount * ((std::max<float>(forward.Dot(moveDirection.Normalized()), threshold)) + 1.0f), ForceType::FORCE);
								}
								else {
									Vector2 directionToMouse = (mousePosition - gameObject->GetComponent<RigidBody2D>().lock()->GetPosition());
									gameObject->GetComponent<RigidBody2D>().lock()->SetRotation(forwardAngle + (forward.AngleToPointInDegrees(directionToMouse) * 0.1f));
									if (input->GetKey(KeyCodes::KeyCode::Up) || input->GetKey(KeyCodes::KeyCode::W)) gameObject->GetComponent<CharacterScript>().lock()->MoveUsingPhysics(Vector2(0.0f, -moveAmount));
									if (input->GetKey(KeyCodes::KeyCode::Down) || input->GetKey(KeyCodes::KeyCode::S)) gameObject->GetComponent<CharacterScript>().lock()->MoveUsingPhysics(Vector2(0.0f, moveAmount));
									if (input->GetKey(KeyCodes::KeyCode::Left) || input->GetKey(KeyCodes::KeyCode::A)) gameObject->GetComponent<CharacterScript>().lock()->MoveUsingPhysics(Vector2(-moveAmount, 0.0f));
									if (input->GetKey(KeyCodes::KeyCode::Right) || input->GetKey(KeyCodes::KeyCode::D)) gameObject->GetComponent<CharacterScript>().lock()->MoveUsingPhysics(Vector2(moveAmount, 0.0f));
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

	void PlayerScript::OnCollisionEnter(const CollisionData & data) {
		//std::shared_ptr<GameObject> g = data.gameObject.lock();
		//std::shared_ptr<GameObject> mg = gameObject.lock();
		//if(g && mg) std::cout << mg->GetName() + " Collided with: " + g->GetName() << std::endl;
	}

	void PlayerScript::OnCollisionExit(const CollisionData & data) {
		/*std::shared_ptr<GameObject> g = data.gameObject.lock();
		std::shared_ptr<GameObject> mg = gameObject.lock();
		if(g && mg) std::cout << mg->GetName() + " Stopped Collided with: " + g->GetName() << std::endl;*/
	}

	void PlayerScript::OnSensorEnter(const std::weak_ptr<Collider>& collider) {


	}

	void PlayerScript::OnSensorExit(const std::weak_ptr<Collider>& collider) {
	}

	void PlayerScript::SetCharacterEnabled(const bool & enabled) {
		std::shared_ptr<GameObject> p = player.lock();
		if (p) {
			std::shared_ptr<RigidBody2D> rb = p->GetComponent<RigidBody2D>().lock();
			if (rb) rb->SetEnabled(enabled);
			std::shared_ptr<SpriteRenderer> sr = p->GetComponent<SpriteRenderer>().lock();
			if (sr) sr->SetEnabled(enabled);
			std::shared_ptr<HealthScript> hs = p->GetComponent<HealthScript>().lock();
			if (hs) hs->SetEnabled(enabled);
			std::shared_ptr<CharacterScript> cs = p->GetComponent<CharacterScript>().lock();
			if (cs) cs->SetEnabled(enabled);
			std::shared_ptr<BloodSplatterScript> bs = p->GetComponent<BloodSplatterScript>().lock();
			if (bs) bs->SetEnabled(enabled);
			std::shared_ptr<Transform2D> hand = p->GetComponent<Transform2D>().lock()->GetChild(0).lock();
			if (hand) hand->GetGameObject().lock()->SetEnabled(enabled);
		}
	}

	void PlayerScript::SetDriving(const bool & driving) {
		if (driving) {
			std::shared_ptr<VehicleController> c = car.lock();
			if (c) {

				if (!this->driving) {
					this->driving = true;
				}
				std::shared_ptr<GameObject> p = player.lock();
				if (p) {
					std::shared_ptr<AttackerIdentityScript> ais = car.lock()->GetComponent<AttackerIdentityScript>().lock();
					std::shared_ptr<CharacterScript> cs = p->GetComponent<CharacterScript>().lock();
					if (ais && cs) {
						ais->SetAttacker(cs->GetGameObject(), cs->GetCharacterName(), AttackType::VehicleHit);
					}
				}

			}

		}
		else if (this->driving) {
			std::shared_ptr<VehicleController> c = car.lock();
			std::shared_ptr<GameObject> p = player.lock();
			this->driving = false;
			if (c) {
				std::shared_ptr<AttackerIdentityScript> ais = c->GetComponent<AttackerIdentityScript>().lock();
				if (ais) {
					ais->SetAttacker();
				}
				if (p) {
					std::shared_ptr<RigidBody2D> rb = p->GetComponent<RigidBody2D>().lock();
					if (c) {
						std::shared_ptr<RigidBody2D> carRB = c->GetComponent<RigidBody2D>().lock();
						rb->SetPosition(carRB->GetPosition() - (carRB->GetRight() * Physics::PIXELS_PER_METRE) + (carRB->GetForward() * Physics::PIXELS_PER_METRE));
						rb->AddForce(carRB->GetVelocity().RotatedInDegrees(-45.0f) * 0.1f);
					}
				}
			}
		}
		SetCharacterEnabled(!driving);
	}
}
