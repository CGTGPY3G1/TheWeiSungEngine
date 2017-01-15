#include "TestScene.h"
#include "Engine.h"
#include "Input.h"
#include "Time.h"
#include "Graphics.h"
#include "GameObject.h"
#include "GameObjectManager.h"
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

TestScene::TestScene() : Scene() {
}

TestScene::~TestScene() {
}

std::weak_ptr<GameObject> TestScene::CreateCharacter(const std::string & name, const int & characterType, const Vector2 & position, const Vector2 & scale, const float & rotation) {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
	std::shared_ptr<GameObject> character = gameObjectManager.CreateGameObject(name).lock();
	character->Init(position, rotation, scale);
	if(characterType != 0) {
		int mask = character->GetCollisionMask();
		mask ^= CollisionCategory::CATEGORY_AI_CHARACTER;
		character->SetCollisionFilter(CollisionCategory::CATEGORY_AI_CHARACTER, mask);
	}
	else {
		character->SetCollisionCategory(CollisionCategory::CATEGORY_PLAYER);
	}
	std::shared_ptr<RigidBody2D> r = character->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_dynamicBody, false, 0.5f, 1.0f);
	std::shared_ptr<CircleCollider> c = character->AddComponent<CircleCollider>().lock();
	c->Init(Vector2(0.0f, 0.0f), 14.0f, false);
	std::shared_ptr<PolygonCollider> body = character->AddComponent<PolygonCollider>().lock();
	body->Init(Vector2(), {Vector2(-7.0f, -28.0f), Vector2(-14.0f, 0.0f), Vector2(-7.0f, 28.0f), Vector2(-5.0f, 29.0f), Vector2(1.0f, 28.0f), Vector2(8.0f, 0.0f), Vector2(1.0f, -28.0f), Vector2(-5.0f, -29.0f)});
	std::shared_ptr<SpriteRenderer> sr = character->AddComponent<SpriteRenderer>().lock();
	sr->Init("Images/Characters.png", PivotPoint::Centre, RenderLayer::FOREGROUND_LAYER, 85);
	sr->SetTextureRect(64 * characterType, 0, 64, 64);
	r->SetMass(10);
	character->AddComponent<CharacterMovementScript>().lock()->Start();
	
	return character;
}

void TestScene::Start() {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
	Scene::Start();
	Engine::GetInstance().GetInput().lock()->SetControllerActive(0, false);

	Vector2 scale = Vector2(6.0f, 6.0f);
	std::shared_ptr<GameObject> tileset = gameObjectManager.CreateGameObject("BackgroundTileset").lock();
	tileset->Init(Vector2(0, 0), 0, scale);
	std::shared_ptr<TileMapper> tileMapper = tileset->AddComponent<TileMapper>().lock();
	tileMapper->Init("Tileset1", "NewTiles");


	static const std::string vertShader = "";
	/*	\
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
	
	std::shared_ptr<GameObject> car = gameObjectManager.CreateGameObject("Car").lock();
	car->Init();
	
	
	std::shared_ptr<Transform2D> carTransform = car->GetComponent<Transform2D>().lock();
	carTransform->SetPosition(Vector2(600.0f, -300.0f));
	std::shared_ptr<RigidBody2D> carRB = car->AddComponent<RigidBody2D>().lock();
	carRB->Init(b2BodyType::b2_dynamicBody, false, 1.0f, 1.0f);
	car->AddComponent<PolygonCollider>().lock()->Init(Vector2(-32.0f, 1.0f), {Vector2(-32.0f, -26.0f), Vector2(-14.0f, -52.0f), Vector2(34.0f, -58.0f), Vector2(64.0f, 50.0f), Vector2(64.0f, -50.0f), Vector2(34.0f, 58.0f), Vector2(-14.0f, 52.0f), Vector2(-32.0f, 26.0f)}, false, 500.0f);
	car->AddComponent<PolygonCollider>().lock()->Init(Vector2(96.0f, 0.0f), {Vector2(-100.0f, -48.0f), Vector2(76.0f, -64.0f), Vector2(112.0f, -38.0f), Vector2(124.0f, -24.0f), Vector2(124.0f, 24.0f), Vector2(112.0f, 38.0f), Vector2(76.0f, 64.0f), Vector2(-100.0f, 48.0f)}, false, 100.0f);
	//carRB->SetMass(1500.0f);
	std::shared_ptr<SpriteRenderer> carSprite = car->AddComponent<SpriteRenderer>().lock();
	carSprite->Init("Images/Car.png", PivotPoint::Left, RenderLayer::MIDGROUND_LAYER);
	carSprite->SetPivotManually(64.0f, carSprite->GetHeight() / 2.0f);
	car->AddComponent<VehicleController>().lock()->Start();
	/*car->GetComponent<SpriteRenderer>().lock()->SetShader(shader);
	car->GetComponent<SpriteRenderer>().lock()->GetShader()->setUniform("texture", sf::Shader::CurrentTexture);*/
	
	const float ppm = Physics::PIXELS_PER_METRE;
	std::shared_ptr<GameObject> boundaries = gameObjectManager.CreateGameObject("Boundaries").lock();
	boundaries->Init();
	std::shared_ptr<RigidBody2D> rb = boundaries->AddComponent<RigidBody2D>().lock();
	rb->Init(b2BodyType::b2_kinematicBody);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(0, -736 * scale.y), Vector2(2880 * scale.x, ppm * 5), false);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(-1408 * scale.x, 8), Vector2(ppm * 5, 1504 * scale.y), false);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(0, 736 * scale.y), Vector2(2880 * scale.x, ppm * 5), false);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(1408 * scale.x, 0), Vector2(ppm * 5, 1504 * scale.y), false);
	const float left = -38.5f * scale.x * 32.0f, top = - 17.5f * scale.y * 32.0f;
	const float right = -left, bottom = -top;
	for(size_t i = 0; i < 50; i++) {
		std::shared_ptr<GameObject> tg = CreateCharacter("Civ", 1, Vector2(Random::RandomFloat(left, right), top)).lock();
		std::shared_ptr<CivWaypointScript> cs = tg->AddComponent<CivWaypointScript>().lock();
		cs->Start();
		cs->SetTarget(Random::RandomBool() ? Vector2(right, top) : Vector2(left, top));
		cs->SetExtents(left, top, right, bottom);

		tg = CreateCharacter("Civ", 1, Vector2(Random::RandomFloat(left, right), bottom)).lock();
		cs = tg->AddComponent<CivWaypointScript>().lock();
		cs->Start();
		cs->SetTarget(Random::RandomBool() ? Vector2(right, bottom) : Vector2(left, bottom));
		cs->SetExtents(left, top, right, bottom);

		tg = CreateCharacter("Civ", 1, Vector2(left, Random::RandomFloat(top, bottom))).lock();
		cs = tg->AddComponent<CivWaypointScript>().lock();
		cs->Start();
		cs->SetTarget(Random::RandomBool() ? Vector2(left, top) : Vector2(left, bottom));
		cs->SetExtents(left, top, right, bottom);

		tg = CreateCharacter("Civ", 1, Vector2(right, Random::RandomFloat(top, bottom))).lock();
		cs = tg->AddComponent<CivWaypointScript>().lock();
		cs->Start();
		cs->SetTarget(Random::RandomBool() ? Vector2(right, top) : Vector2(right, bottom));
		cs->SetExtents(left, top, right, bottom);
	}
	
}


void TestScene::FixedUpdate(const float & fixedDeltaTime) {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
	Engine engine = Engine::GetInstance();
	std::shared_ptr<Input> input = engine.GetInput().lock();
	std::shared_ptr<GameObject> gameObject = driving ? gameObjectManager.GetGameObject("Car").lock() : gameObjectManager.GetGameObject("Player").lock();
	std::shared_ptr<Transform2D> playerTransform = gameObject->GetComponent<Transform2D>().lock();
	bool useController = input->GetControllerActive(0);
	RayCastHit raycastHit = PhysicsSystem::GetInstance().RayCast(playerTransform->GetPosition(), playerTransform->GetPosition() + playerTransform->GetForward() * 400);
	// Movement Test
	std::shared_ptr<RigidBody2D> rb = gameObject->GetComponent<RigidBody2D>().lock();
	float dot = rb->GetVelocity().Dot(rb->GetForward());
	dot = (dot < 0.0001f) ? -1.0f : 1.0f;
	const float cameraScale = (dot < 0.0f) ? -0.4f : dot;
	engine.GetGraphics().lock()->MoveCamera((driving ? ((rb->GetPosition() + (rb->GetForward() * cameraScale * rb->GetSpeed())) - engine.GetGraphics().lock()->GetCameraPosition()) : (gameObject->GetComponent<RigidBody2D>().lock()->GetPosition() - engine.GetGraphics().lock()->GetCameraPosition())) * (fixedDeltaTime * 1.5f));
	const float maxVelocity = Physics::PIXELS_PER_METRE * 2.0f;
	const float speed = gameObject->GetComponent<RigidBody2D>().lock()->GetSpeed();
	float oldZoom = engine.GetGraphics().lock()->GetCameraZoom();
	float newZoom = (speed / maxVelocity) * 0.4f;
	engine.GetGraphics().lock()->SetCameraZoom(std::max<float>(1.0f, (oldZoom * (1.0f - fixedDeltaTime) + ((1.0f + newZoom) * fixedDeltaTime))));
	if(driving) {
		std::shared_ptr<VehicleController> vehicle = gameObject->GetComponent<VehicleController>().lock();
		if(vehicle) {
			if(useController) {
				float force = 200.0f * input->GetAxis(0, ControllerButtons::ControllerAxis::RT);
				force -= 40.0f * input->GetAxis(0, ControllerButtons::ControllerAxis::LT);
				vehicle->Drive(force);
				vehicle->Steer(input->GetAxis(0, ControllerButtons::ControllerAxis::LeftStickH));
			}
			else {
				if(input->GetKey(KeyCodes::KeyCode::W) || input->GetKey(KeyCodes::KeyCode::Up))
					vehicle->Drive(200.0f);
				else if(input->GetKey(KeyCodes::KeyCode::S) || input->GetKey(KeyCodes::KeyCode::Down)) 
					vehicle->Drive(-40.0f);
				
				const float turnAngle = 1.0f;
				if(input->GetKey(KeyCodes::KeyCode::A) || input->GetKey(KeyCodes::KeyCode::Left)) 
					vehicle->Steer(-turnAngle);
				else if(input->GetKey(KeyCodes::KeyCode::D) || input->GetKey(KeyCodes::KeyCode::Right)) 
					vehicle->Steer(turnAngle);
			}
		}
	}
	else {
		Vector2 forward = gameObject->GetComponent<RigidBody2D>().lock()->GetForward();
		const float forwardAngle = forward.AngleInDegrees();
		float moveAmount = gameObject->GetComponent<RigidBody2D>().lock()->GetMass() * 2.0f;
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
			if(oldInputStyle) {
				if(input->GetKey(KeyCodes::KeyCode::Up) || input->GetKey(KeyCodes::KeyCode::W)) gameObject->GetComponent<CharacterMovementScript>().lock()->MoveUsingPhysics(Vector2(0.0f, -moveAmount));
				if(input->GetKey(KeyCodes::KeyCode::Down) || input->GetKey(KeyCodes::KeyCode::S)) gameObject->GetComponent<CharacterMovementScript>().lock()->MoveUsingPhysics(Vector2(0.0f, moveAmount));
				if(input->GetKey(KeyCodes::KeyCode::Left) || input->GetKey(KeyCodes::KeyCode::A)) gameObject->GetComponent<CharacterMovementScript>().lock()->MoveUsingPhysics(Vector2(-moveAmount, 0.0f));
				if(input->GetKey(KeyCodes::KeyCode::Right) || input->GetKey(KeyCodes::KeyCode::D)) gameObject->GetComponent<CharacterMovementScript>().lock()->MoveUsingPhysics(Vector2(moveAmount, 0.0f));
			}
			else {
				if(input->GetKey(KeyCodes::KeyCode::Up) || input->GetKey(KeyCodes::KeyCode::W)) gameObject->GetComponent<CharacterMovementScript>().lock()->MoveUsingPhysics(Vector2(moveAmount, 0.0f), false);
				if(input->GetKey(KeyCodes::KeyCode::Down) || input->GetKey(KeyCodes::KeyCode::S)) gameObject->GetComponent<CharacterMovementScript>().lock()->MoveUsingPhysics(Vector2(-moveAmount, 0.0f), false);
				if(input->GetKey(KeyCodes::KeyCode::Left) || input->GetKey(KeyCodes::KeyCode::A)) gameObject->GetComponent<CharacterMovementScript>().lock()->MoveUsingPhysics(Vector2(0.0f, -moveAmount), false);
				if(input->GetKey(KeyCodes::KeyCode::Right) || input->GetKey(KeyCodes::KeyCode::D)) gameObject->GetComponent<CharacterMovementScript>().lock()->MoveUsingPhysics(Vector2(0.0f, moveAmount), false);
			}
		}
	}
	Scene::FixedUpdate(fixedDeltaTime);
}

void TestScene::Update(const float & deltaTime) {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();

	
	Scene::Update(deltaTime);
	std::shared_ptr<Input> input = Engine::GetInstance().GetInput().lock();
	mousePosition = input->GetMousePosition();
	if(input->GetKeyDown(KeyCodes::KeyCode::C)) drawColliders = !drawColliders;
	if(input->GetKeyDown(KeyCodes::KeyCode::G)) input->SetControllerActive(0, !input->GetControllerActive(0));
	if(input->GetKeyDown(KeyCodes::KeyCode::I)) oldInputStyle = !oldInputStyle;
	std::shared_ptr<GameObject> player = gameObjectManager.GetGameObject("Player").lock();
	std::shared_ptr<GameObject> car = gameObjectManager.GetGameObject("Car").lock();
	if(!driving) {
		if((car->GetComponent<Transform2D>().lock()->GetPosition() - player->GetComponent<Transform2D>().lock()->GetPosition() - player->GetComponent<Transform2D>().lock()->GetForward() * Physics::PIXELS_PER_METRE).Magnitude() < Physics::PIXELS_PER_METRE * 2.0f) {
			if(input->GetKeyDown(KeyCodes::KeyCode::E) || input->GetKeyDown(KeyCodes::KeyCode::Space) || input->GetControllerButtonDown(0, ControllerButtons::ControllerButton::Y)) {
				driving = true;
				player->SetEnabled(false);
				player->GetComponent<RigidBody2D>().lock()->SetEnabled(false);
			}
		}
	}
	else {
		if(input->GetKeyDown(KeyCodes::KeyCode::E) || input->GetKeyDown(KeyCodes::KeyCode::Space) || input->GetControllerButtonDown(0, ControllerButtons::ControllerButton::Y)) {
			std::shared_ptr<Transform2D> carTransform = car->GetComponent<Transform2D>().lock();
			driving = false;
			player->GetComponent<RigidBody2D>().lock()->SetPosition(carTransform->GetPosition() + (carTransform->GetRight() * Physics::PIXELS_PER_METRE) + (carTransform->GetForward() * Physics::PIXELS_PER_METRE));
			player->SetEnabled(true);
			player->GetComponent<RigidBody2D>().lock()->SetEnabled(true);
		}
	}
}

void TestScene::Render() {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
	std::shared_ptr<GameObject> tileset = gameObjectManager.GetGameObject("BackgroundTileset").lock();
	if(tileset) {
		std::shared_ptr<TileMapper> tileMapper = tileset->GetComponent<TileMapper>().lock();
		tileMapper->Draw();
	}
	Engine engine = Engine::GetInstance();
	Scene::Render();
	float zoom = engine.GetGraphics().lock()->GetCameraZoom();
	/*float speed = gameObjectManager.GetGameObject("Car").lock()->GetComponent<RigidBody2D>().lock()->GetSpeed() * Physics::METRES_PER_PIXEL;
	speed *= 1.0f / 1000.0f;
	speed *= 60 * 60;
	speed *= 0.621371f;
	engine.GetGraphics().lock()->Draw("Car Speed = " + std::to_string(speed), Vector2(100.0f, 650.0f), 30.0f * zoom);*/

	engine.GetGraphics().lock()->Draw("Total Time = " + std::to_string(engine.GetTimer().lock()->GetTotalTime()) + "	Delta Time = " + std::to_string(engine.GetTimer().lock()->GetDeltaTime()) + "	FPS = " + std::to_string(engine.GetFPS()), Vector2(100.0f, 650.0f), (unsigned int)(30.0f * zoom));
	
}
