#include "TestScene.h"
#include "Engine.h"
#include "Input.h"
#include "Time.h"
#include "Graphics.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "GameObjectFactory.h"
#include "PhysicsSystem.h"
#include <random>
#include <algorithm>
#include "Math.h"
#include "Random.h"
#include "Debug.h"
#include "FileManager.h"
#include "TileMapper.h"
#include <memory>
#include "EngineSettings.h"
#include "PopulationController.h"
#include <SFML\Audio\Listener.hpp>
#include "AssetManager.h"
#include "WeaponCache.h"
#include "RigidBody2D.h"


namespace WeiSungEngine {
	sf::Sprite healthSprite, weaponOverlay, weaponSprite;
	void TestScene::Detonate(Vector2 position, float radius, float explosionForce, float damage) {
		std::vector<std::weak_ptr<Collider>> colliders = PhysicsSystem::GetInstance().CircleCast(position, radius);
		for (std::vector<std::weak_ptr<Collider>>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
			std::shared_ptr<Collider> c = (*it).lock();
			if (c) {
				std::shared_ptr<RigidBody2D> rb = c->GetComponent<RigidBody2D>().lock();
				if (rb) {
					Vector2 cPosition = rb->GetPosition();
					const float distance = (cPosition - position).Magnitude();
					Vector2 direction = (cPosition - position).Normalized();
					rb->AddForce(direction * (radius / (distance != 0 ? distance : radius)) * explosionForce, ForceType::IMPULSE_FORCE);
				}
			}
		}
	}

	TestScene::TestScene() : Scene() {
	}

	TestScene::~TestScene() {
	}

	void TestScene::Start() {

		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		Scene::Start();
		Engine::GetInstance().GetInput().lock()->SetControllerActive(0, false);


		healthSprite.setTexture(AssetManager::GetInstance().GetTexture("Images/health.png"));
		weaponOverlay.setTexture(AssetManager::GetInstance().GetTexture("Images/WeaponOverlay.png"));
		weaponSprite.setTexture(AssetManager::GetInstance().GetTexture("Images/Items.png"));
		Vector2 scale = Vector2(4.0f, 4.0f);
		std::shared_ptr<GameObject> tileset = gameObjectManager.CreateGameObject("Tileset").lock();
		tileset->Init(Vector2(0, 0), 0, scale);
		std::shared_ptr<TileMapper> tileMapper = tileset->AddComponent<TileMapper>().lock();
		tileMapper->Init(EngineSettings::TILESET_FILE_NAME, EngineSettings::TILESET_NAME);

		Engine::GetInstance().GetGraphics().lock()->SetCameraPosition(gameObjectManager.GetGameObject("Player").lock()->GetComponent<Transform2D>().lock()->GetPosition());
		Vector2 rampagePosition = Vector2(26000.0f, 6100.0f);

		/*static const std::string vertShader = \
			"void main(){"\
			"   gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;"\
			"   gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"\
			"   gl_FrontColor = gl_Color;}";*/


			/*static const std::string fragShader = \
				"uniform sampler2D currentTexture;"\
				"uniform sampler2D distortionMapTexture;"\
				"uniform float time;"\
				"uniform float distortionFactor;"\
				"uniform float riseFactor;"\
				"void main() {"\
				"vec2 distortionMapCoordinate = gl_TexCoord[0].st;"\
				"distortionMapCoordinate.t -= time * riseFactor;"\
				"vec4 distortionMapValue = texture2D(distortionMapTexture, distortionMapCoordinate);"\
				"vec2 distortionPositionOffset = distortionMapValue.xy;"\
				"distortionPositionOffset -= vec2(0.5, 0.5);"\
				"distortionPositionOffset *= 2.;"\
				"distortionPositionOffset *= distortionFactor;"\
				"vec2 distortionUnused = distortionMapValue.zw;"\
				"distortionPositionOffset *= (1. - gl_TexCoord[0].t);"\
				"vec2 distortedTextureCoordinate = gl_TexCoord[0].st + distortionPositionOffset;"\
				"gl_FragColor = gl_Color * texture2D(currentTexture, distortedTextureCoordinate);}";*/

				/*static const std::string fragShader = \
					"uniform sampler2D texture;"\
					"uniform float blur_radius;"\
					"void main() {"\
					"vec2 offx = vec2(blur_radius, 0.0);"\
					"vec2 offy = vec2(0.0, blur_radius);"\
					"vec4 pixel = texture2D(texture, gl_TexCoord[0].xy)               * 4.0 +"\
					"texture2D(texture, gl_TexCoord[0].xy - offx)        * 2.0 +"\
					"texture2D(texture, gl_TexCoord[0].xy + offx)        * 2.0 +"\
					"texture2D(texture, gl_TexCoord[0].xy - offy)        * 2.0 +"\
					"texture2D(texture, gl_TexCoord[0].xy + offy)        * 2.0 +"\
					"texture2D(texture, gl_TexCoord[0].xy - offx - offy) * 1.0 +"\
					"texture2D(texture, gl_TexCoord[0].xy - offx + offy) * 1.0 +"\
					"texture2D(texture, gl_TexCoord[0].xy + offx - offy) * 1.0 +"\
					"texture2D(texture, gl_TexCoord[0].xy + offx + offy) * 1.0;"\
					"gl_FragColor = gl_Color * (pixel / 16.0);}";


				std::shared_ptr<sf::Shader> shader = std::make_shared<sf::Shader>();
				shader->loadFromMemory(fragShader, sf::Shader::Fragment);*/

				/*car->GetComponent<SpriteRenderer>().lock()->SetShader(shader);
				car->GetComponent<SpriteRenderer>().lock()->GetShader()->setUniform("texture", sf::Shader::CurrentTexture);*/

	}


	void TestScene::FixedUpdate(const float & fixedDeltaTime) {
		Scene::FixedUpdate(fixedDeltaTime);
	}

	void TestScene::Update(const float & deltaTime) {
		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		GameObjectFactory::CreateScheduledCollectables();
		Scene::Update(deltaTime);
		std::shared_ptr<Input> input = Engine::GetInstance().GetInput().lock();
		mousePosition = input->GetMousePosition();
		if (input->GetKeyDown(KeyCodes::KeyCode::F)) printBMF = !printBMF;
		if (input->GetMouseButtonDown(MouseButtons::Right)) Detonate(mousePosition, 5 * Physics::PIXELS_PER_METRE, 10.0f, 100.0f);
		if (input->GetKeyDown(KeyCodes::KeyCode::C)) drawColliders = !drawColliders;
		if (input->GetKeyDown(KeyCodes::KeyCode::G)) input->SetControllerActive(0, !input->GetControllerActive(0));
		if (input->GetKeyDown(KeyCodes::KeyCode::I)) oldInputStyle = !oldInputStyle;

		std::shared_ptr<TileMapper> tileset = gameObjectManager.GetGameObject("Tileset").lock()->GetComponent<TileMapper>().lock();
		if (input->GetKeyDown(KeyCodes::KeyCode::N)) tileset->SetShowNavLinks(!tileset->GetShowNavLinks());
		if (input->GetKeyDown(KeyCodes::KeyCode::L)) tileset->SetShowGridLinks(!tileset->GetShowGridLinks());
		PopulationController::GetInstance().Update();
		std::shared_ptr<GameObject> p = gameObjectManager.GetGameObject("Player", true).lock();
		if (p) playerHealthPercentage = std::min(100, std::max((int)0, (int)std::ceill(p->GetComponent<HealthScript>().lock()->GetHealthAsPercentage())));
		else {
			if (camTarget.use_count() == 0) {
				std::shared_ptr<GameObject> g = PopulationController::GetInstance().GetRandomCiv().lock();
				if (g) {
					camTarget = g->GetComponent<RigidBody2D>().lock();
				}
			}
			else {
				std::shared_ptr<RigidBody2D> rb = camTarget.lock();
				if (rb) {
					float dot = rb->GetVelocity().Dot(rb->GetForward());
					dot = (dot < 0.0001f) ? -1.0f : 1.0f;
					const float cameraScale = (dot < 0.0f) ? -0.4f : dot;
					const Vector2 camPos = rb->GetPosition() - Engine::GetInstance().GetGraphics().lock()->GetCameraPosition();
					Engine::GetInstance().GetGraphics().lock()->MoveCamera((driving ? (camPos + (rb->GetForward() * cameraScale * rb->GetSpeed())) : camPos) * (deltaTime * 1.5f));
					const float maxVelocity = Physics::PIXELS_PER_METRE * 3.0f;
					const float speed = rb->GetSpeed();
					float oldZoom = Engine::GetInstance().GetGraphics().lock()->GetCameraZoom();
					float newZoom = (speed / maxVelocity) * 0.4f;
					const Vector2 listenerPosition = Engine::GetInstance().GetGraphics().lock()->GetCameraPosition();
					sf::Listener::setDirection(0.0f, 0.0f, -1.0f);
					sf::Listener::setUpVector(0.0f, 1.0f, 0.0f);
					sf::Listener::setPosition(listenerPosition.x, 0.0f, listenerPosition.y);
					Engine::GetInstance().GetGraphics().lock()->SetCameraZoom(std::max<float>(1.5f, (oldZoom * (1.0f - deltaTime) + ((1.5f + newZoom) * deltaTime))));
				}
			}
			if (input->GetKeyDown(KeyCodes::KeyCode::R)) GameObjectFactory::CreateCharacter("Player", 0, false, playerPosition, Vector2::One, 0.0f);
		}
	}

	TileType oldTile = TileType::TILE_TYPE_NULL;

	void TestScene::Render() {
		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		std::shared_ptr<GameObject> tileset = gameObjectManager.GetGameObject("Tileset").lock();
		if (tileset) {

			std::shared_ptr<TileMapper> tileMapper = tileset->GetComponent<TileMapper>().lock();
			tileMapper->Draw();
		}
		Scene::Render();
		std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
		std::string healthSring = std::to_string(playerHealthPercentage) + "%";
		graphics->DrawToGUI(healthSprite, Vector2(20.0f, 20.0f), Vector2(80.0f, 80.0f));
		graphics->DrawToGUI(weaponOverlay, Vector2(1100.0f, 20.0f), Vector2(160.0f, 160.0f));
		std::shared_ptr<GameObject> p = gameObjectManager.GetGameObject("Player", true).lock();
		if (p) {
			playerPosition = p->GetComponent<Transform2D>().lock()->GetPosition();
			std::shared_ptr<CharacterScript> cs = p->GetComponent<CharacterScript>().lock();
			if (cs->IsArmed()) {
				std::shared_ptr<WeaponCache> wc = cs->GetWeaponCache().lock();
				if (wc) {
					WeaponType wType = wc->GetWeaponType();
					switch (wType) {
					case WeaponTypePistol:
						weaponSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
						break;
					case WeaponTypeUzi:
						weaponSprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
						break;
					case WeaponTypeGrenade:
						weaponSprite.setTextureRect(sf::IntRect(64, 0, 32, 32));
						break;
					default:
						break;
					}
					graphics->DrawToGUI(weaponSprite, Vector2(1120.0f, 40.0f), Vector2(120.0f, 120.0f));
					std::string ammo = std::to_string(wc->GetAmmoInClip()) + "/" + std::to_string(wc->GetAmmo());
					graphics->Draw(ammo, Vector2(1250.0f, 150.0f), Vector2(ammo.size() * 15.0f, 20.0f), 1.0f, 1.0f, 1.0f, 1.0f, RIGHT_ALIGNED);
				}
			}
		}
		graphics->Draw(healthSring, Vector2(100.0f, 20.0f), Vector2(healthSring.size() * 60.0f, 80.0f), 1.0f, 0.0f, 0.0f, 1.0f, LEFT_ALIGNED);
		std::string pointsAsString = "Score = " + std::to_string(PopulationController::GetInstance().GetScore("Player"));
		graphics->Draw(pointsAsString, Vector2(20.0f, 620.0f), Vector2(pointsAsString.size() * 50.0f, 80.0f), 0.0f, 0.0f, 0.0f, 1.0f, LEFT_ALIGNED);
	}

	void TestScene::SetCamTarget(const std::weak_ptr<RigidBody2D> & camTarget) {
		this->camTarget = camTarget;
	}
}
