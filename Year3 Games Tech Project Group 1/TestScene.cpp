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
#include "CerealTypeRegistration.h"
#include "EngineSettings.h"
#include "PopulationController.h"
#include <SFML\Audio\Listener.hpp>

void TestScene::Detonate(Vector2 position, float radius, float explosionForce, float damage) {
	std::vector<std::weak_ptr<Collider>> colliders = PhysicsSystem::GetInstance().CircleCast(position, radius);
	for(std::vector<std::weak_ptr<Collider>>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		std::shared_ptr<Collider> c = (*it).lock();
		if(c) {
			std::shared_ptr<RigidBody2D> rb = c->GetComponent<RigidBody2D>().lock();
			if(rb) {
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

	Vector2 scale = Vector2(4.0f, 4.0f);
	std::shared_ptr<GameObject> tileset = gameObjectManager.CreateGameObject("Tileset").lock();
	tileset->Init(Vector2(0, 0), 0, scale);
	std::shared_ptr<TileMapper> tileMapper = tileset->AddComponent<TileMapper>().lock();
	tileMapper->Init(EngineSettings::TILESET_FILE_NAME, EngineSettings::TILESET_NAME);

	Engine::GetInstance().GetGraphics().lock()->SetCameraPosition(gameObjectManager.GetGameObject("Player").lock()->GetComponent<Transform2D>().lock()->GetPosition());
	Vector2 rampagePosition = Vector2(26000.0f, 6100.0f);
	std::shared_ptr<GameObject> rampage = gameObjectManager.CreateGameObject("Rampage").lock();
	rampage->Init(rampagePosition, 0.0f, scale);
	std::shared_ptr<RampageScript> rs = rampage->AddComponent<RampageScript>().lock();
	rs->Start();
	rs->SetRequiredKills(50);
	rs->SetRunTime(60.0f);

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
	
	Scene::Update(deltaTime);
	std::shared_ptr<Input> input = Engine::GetInstance().GetInput().lock();
	mousePosition = input->GetMousePosition();
	if(input->GetKeyDown(KeyCodes::KeyCode::E)) {
		std::shared_ptr<GameObject> player = gameObjectManager.GetGameObject("Player").lock(), rampage = gameObjectManager.GetGameObject("Rampage").lock();
		if(player && rampage) {
			std::shared_ptr<Transform2D> pt = player->GetComponent<Transform2D>().lock(), rt = rampage->GetComponent<Transform2D>().lock();
			if(pt && rt) {
				if((rt->GetPosition() - pt->GetPosition()).Magnitude() < 160.0f) {
					std::shared_ptr<RampageScript> rs = rampage->GetComponent<RampageScript>().lock();
					if(rs && !rs->IsActive()) {
						rs->Activate(pt);
					}
				}
			}
		}
	}
	if(input->GetKeyDown(KeyCodes::KeyCode::F)) printBMF = !printBMF;
	if(input->GetMouseButtonDown(MouseButtons::Right)) Detonate(mousePosition, 5 * Physics::PIXELS_PER_METRE, 10.0f, 100.0f);
	if(input->GetKeyDown(KeyCodes::KeyCode::C)) drawColliders = !drawColliders;
	if(input->GetKeyDown(KeyCodes::KeyCode::G)) input->SetControllerActive(0, !input->GetControllerActive(0));
	if(input->GetKeyDown(KeyCodes::KeyCode::I)) oldInputStyle = !oldInputStyle;

	std::shared_ptr<TileMapper> tileset = gameObjectManager.GetGameObject("Tileset").lock()->GetComponent<TileMapper>().lock();
	if(input->GetKeyDown(KeyCodes::KeyCode::N)) tileset->SetShowNavLinks(!tileset->GetShowNavLinks());
	if(input->GetKeyDown(KeyCodes::KeyCode::L)) tileset->SetShowGridLinks(!tileset->GetShowGridLinks());
	PopulationController::GetInstance().Update();
}

TileType oldTile = TileType::TILE_TYPE_NULL;

void TestScene::Render() {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
	std::shared_ptr<GameObject> tileset = gameObjectManager.GetGameObject("Tileset").lock();
	if(tileset) {
		
		std::shared_ptr<TileMapper> tileMapper = tileset->GetComponent<TileMapper>().lock();
		tileMapper->Draw();
		std::shared_ptr<GameObject> p = gameObjectManager.GetGameObject("Player").lock();
		if(p) {
			std::shared_ptr<Transform2D> t = p->GetComponent<Transform2D>().lock();
			sf::Listener::setPosition(1, 0, -5);
			if(t) {
				const Vector2 pos = t->GetPosition();
				TileType currentTile = tileMapper->GetTileType(pos);
				if(oldTile != currentTile) {
					oldTile = currentTile;
					std::cout << tileMapper->GetTileTypeAsString(pos) << std::endl;
					tileMapper->PrintTile(pos);
				}
				sf::Listener::setPosition(pos.x, pos.y, 0);
			}
		}
	}
	Scene::Render();
	if(printBMF) {
		std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
		graphics->Draw("Where's the fucking font?", Vector2(640.0f, 360.0f), Vector2(640.0f, 100.0f), 1.0f, 1.0f, 1.0f, 1.0f, CENTRE_ALIGNED);
	}	
}
