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

TestScene::TestScene() : Scene() {
}

TestScene::~TestScene() {
}

void TestScene::Start() {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
	Scene::Start();
	Engine::GetInstance().GetInput().lock()->SetControllerActive(0, false);

	Vector2 scale = Vector2(4.0f, 4.0f);
	std::shared_ptr<GameObject> tileset = gameObjectManager.CreateGameObject("BackgroundTileset").lock();
	tileset->Init(Vector2(0, 0), 0, scale);
	std::shared_ptr<TileMapper> tileMapper = tileset->AddComponent<TileMapper>().lock();
	tileMapper->Init(EngineSettings::TILESET_FILE_NAME, EngineSettings::TILESET_NAME);

	Engine::GetInstance().GetGraphics().lock()->SetCameraPosition(gameObjectManager.GetGameObject("Player").lock()->GetComponent<Transform2D>().lock()->GetPosition());
	Vector2 rampagePosition = Vector2(16312.0f, 11568.0f);
	std::shared_ptr<GameObject> rampage = gameObjectManager.CreateGameObject("Rampage").lock();
	rampage->Init(rampagePosition, 0.0f, scale);
	std::shared_ptr<RampageScript> rs = rampage->AddComponent<RampageScript>().lock();
	rs->Start();
	rs->SetSpawnRadius(20.0f * Physics::PIXELS_PER_METRE);
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
	
	/*const float ppm = Physics::PIXELS_PER_METRE;
	std::shared_ptr<GameObject> boundaries = gameObjectManager.CreateGameObject("Boundaries").lock();
	boundaries->Init();
	std::shared_ptr<RigidBody2D> rb = boundaries->AddComponent<RigidBody2D>().lock();
	rb->Init(b2BodyType::b2_kinematicBody);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(0, -736 * scale.y), Vector2(2880 * scale.x, ppm * 5), false);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(-1408 * scale.x, 8), Vector2(ppm * 5, 1504 * scale.y), false);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(0, 736 * scale.y), Vector2(2880 * scale.x, ppm * 5), false);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(1408 * scale.x, 0), Vector2(ppm * 5, 1504 * scale.y), false);*/
	/*const float left = -38.5f * scale.x * 32.0f, top = - 17.5f * scale.y * 32.0f;
	const float right = -left, bottom = -top;
	for(size_t i = 0; i < 50; i++) {
		std::shared_ptr<GameObject> tg = GameObjectFactory::CreateCharacter("Civ", 1, Vector2(Random::RandomFloat(left, right), top)).lock();
		std::shared_ptr<CivWaypointScript> cs = tg->AddComponent<CivWaypointScript>().lock();
		cs->Start();
		cs->SetTarget(Random::RandomBool() ? Vector2(right, top) : Vector2(left, top));
		cs->SetExtents(left, top, right, bottom);

		tg = GameObjectFactory::CreateCharacter("Civ", 1, Vector2(Random::RandomFloat(left, right), bottom)).lock();
		cs = tg->AddComponent<CivWaypointScript>().lock();
		cs->Start();
		cs->SetTarget(Random::RandomBool() ? Vector2(right, bottom) : Vector2(left, bottom));
		cs->SetExtents(left, top, right, bottom);

		tg = GameObjectFactory::CreateCharacter("Civ", 1, Vector2(left, Random::RandomFloat(top, bottom))).lock();
		cs = tg->AddComponent<CivWaypointScript>().lock();
		cs->Start();
		cs->SetTarget(Random::RandomBool() ? Vector2(left, top) : Vector2(left, bottom));
		cs->SetExtents(left, top, right, bottom);

		tg = GameObjectFactory::CreateCharacter("Civ", 1, Vector2(right, Random::RandomFloat(top, bottom))).lock();
		cs = tg->AddComponent<CivWaypointScript>().lock();
		cs->Start();
		cs->SetTarget(Random::RandomBool() ? Vector2(right, top) : Vector2(right, bottom));
		cs->SetExtents(left, top, right, bottom);
	}*/
	
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

	if(input->GetKeyDown(KeyCodes::KeyCode::C)) drawColliders = !drawColliders;
	if(input->GetKeyDown(KeyCodes::KeyCode::G)) input->SetControllerActive(0, !input->GetControllerActive(0));
	if(input->GetKeyDown(KeyCodes::KeyCode::I)) oldInputStyle = !oldInputStyle;
}

void TestScene::Render() {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
	std::shared_ptr<GameObject> tileset = gameObjectManager.GetGameObject("BackgroundTileset").lock();
	if(tileset) {
		
		std::shared_ptr<TileMapper> tileMapper = tileset->GetComponent<TileMapper>().lock();
		tileMapper->Draw();
	}
	//Engine engine = Engine::GetInstance();
	Scene::Render();
	//float zoom = engine.GetGraphics().lock()->GetCameraZoom();
	//std::shared_ptr<Transform2D> p = gameObjectManager.GetGameObject("Player").lock()->GetComponent<Transform2D>().lock();
	//engine.GetGraphics().lock()->Draw("Total Time = " + std::to_string(engine.GetTimer().lock()->GetTotalTime()) + "	Delta Time = " + std::to_string(engine.GetTimer().lock()->GetDeltaTime()) + "	FPS = " + std::to_string(engine.GetFPS()), Vector2(100.0f, 650.0f), (unsigned int)(30.0f * zoom));
}
