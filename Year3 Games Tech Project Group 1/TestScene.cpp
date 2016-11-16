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
sf::RectangleShape rect;
sf::CircleShape circle;
TestScene::TestScene() : Scene() {
}

TestScene::~TestScene() {
}

void TestScene::Start() {
	Scene::Start();
	//SetUpShapes();
	std::shared_ptr<GameObject> u = gameObjectManager->CreateGameObject("Player").lock();
	std::shared_ptr<GameObject> v = gameObjectManager->CreateGameObject("Background").lock();
	u->Init();
	v->Init();
	g1 = u;
	g2 = v;
	std::shared_ptr<Transform2D> t1 = u->AddComponent<Transform2D>().lock(); 
	std::shared_ptr<Transform2D> t2 = v->AddComponent<Transform2D>().lock();
	
	t1->SetPosition(Vector2(640.0f, 360.0f));
	std::shared_ptr<RigidBody2D> r = t1->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_dynamicBody, 1.0f, 0.5f, 1.0f);
	r->SetMass(50);
	std::shared_ptr<CircleCollider> c = t1->AddComponent<CircleCollider>().lock();
	c->Init(Vector2(), 20.0f, false);
	std::shared_ptr<CircleCollider> playerSensor = t1->AddComponent<CircleCollider>().lock();
	playerSensor->Init(Vector2(), 200.0f, true);
	std::shared_ptr<SpriteRenderer> sr = u->AddComponent<SpriteRenderer>().lock();
	sr->Init("Images/Ball.png", RenderLayer::FOREGROUND_LAYER, 85);
	t2->SetPosition(Vector2(640.0f, 360.0f));
	/*std::shared_ptr<RigidBody2D> r2 = t2->AddComponent<RigidBody2D>().lock();
	r2->Init(b2BodyType::b2_kinematicBody);
	std::shared_ptr<PolygonCollider> house = t2->AddComponent<PolygonCollider>().lock();
	house->Init(Vector2(-350.0f, 0.0f), {Vector2(-50.0f, 100.0f), Vector2(50.0f, 100.0f), Vector2(50.0f, 0.0f), Vector2(0.0f, -50.0f), Vector2(-50.0f, 0.0f)}, true);
	const float yDist = 384.0f;
	for(size_t i = 0; i < 3; i++) {
		std::shared_ptr<BoxCollider> b = t2->AddComponent<BoxCollider>().lock();
		b->Init(Vector2(512.0f, -yDist + 384.0f * i), Vector2(512.0f, 256.0f));
	}*/
	std::shared_ptr<SpriteRenderer> spriteRenderer = t2->AddComponent<SpriteRenderer>().lock();
	spriteRenderer->Init("Images/Street.png", RenderLayer::BACKGROUND_LAYER, 0);
	spriteRenderer->GetSprite().setScale(sf::Vector2f(2, 2));
	/*Engine & engine = Engine::GetInstance();
	Vector2 res = engine.GetGraphics()->GetScreenResolution();
	const int NUM_X = 100, NUM_Y = 50;
	float offsetX = res.x / NUM_X;
	float offsety = res.y / NUM_Y;
	std::cout << "Creating " + std::to_string(NUM_X * NUM_Y) + " Sprites";
	int layer = 0, sortOrder = 0;
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<> sortRange(-100, 100);
	for(size_t i = 0; i < NUM_X; i++) {
		for(size_t j = 0; j < NUM_Y; j++) {
			if(layer >= RenderLayer::MUNBER_OF_LAYERS) layer = 0;
			sortOrder = sortRange(generator);
			std::shared_ptr<GameObject> testObject = gameObjectManager->CreateGameObject().lock();
			testObject->Init();
			std::shared_ptr<Transform2D> transform = testObject->AddComponent<Transform2D>().lock();
			transform->SetPosition(Vector2(offsetX * i, offsety * j));
			std::shared_ptr<SpriteRenderer> spriteRenderer = testObject->AddComponent<SpriteRenderer>().lock();
			spriteRenderer->Init("Images/Tree.png", (RenderLayer)layer, sortOrder);
			sortOrder--;
			layer++;
		}
	}*/
}

void TestScene::FixedUpdate(const float & fixedDeltaTime) {	
	Engine engine = Engine::GetInstance();
	Input * input = engine.GetInput();
	std::shared_ptr<GameObject> gameObject = gameObjectManager->GetGameObject("Player").lock();
	Vector2 directionToMouse = (mousePosition - gameObject->GetComponent<RigidBody2D>().lock()->GetPosition());
	gameObject->GetComponent<RigidBody2D>().lock()->SetRotation(directionToMouse.AngleInDegrees());
	// Movement Test
	Vector2 forward = Vector2(1, 0).RotateInDegrees(gameObject->GetComponent<RigidBody2D>().lock()->GetRotation());
	float moveAmount = gameObject->GetComponent<RigidBody2D>().lock()->GetMass() * Physics::PIXELS_PER_METRE * 2.0f;
	const float threshold = 0.4f;
	if(input->GetKey(KeyCodes::KeyCode::Up) || input->GetKey(KeyCodes::KeyCode::W)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(0.0f, -moveAmount * ((std::max<float>(forward.Dot(Vector2(0, 1)), threshold)) + 1.0f)), ForceType::FORCE);
	if(input->GetKey(KeyCodes::KeyCode::Down) || input->GetKey(KeyCodes::KeyCode::S)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(0.0f, moveAmount * ((std::max<float>(forward.Dot(Vector2(0, -1)), threshold)) + 1.0f)), ForceType::FORCE);
	if(input->GetKey(KeyCodes::KeyCode::Left) || input->GetKey(KeyCodes::KeyCode::A)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(-moveAmount * ((std::max<float>(forward.Dot(Vector2(-1, 0)), threshold)) + 1.0f), 0.0f), ForceType::FORCE);
	if(input->GetKey(KeyCodes::KeyCode::Right) || input->GetKey(KeyCodes::KeyCode::D)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(moveAmount * ((std::max<float>(forward.Dot(Vector2(1, 0)), threshold)) + 1.0f), 0.0f), ForceType::FORCE);
	Scene::FixedUpdate(fixedDeltaTime);
	engine.GetGraphics()->MoveCamera((gameObject->GetComponent<RigidBody2D>().lock()->GetPosition() - engine.GetGraphics()->GetCameraPosition()) *  (fixedDeltaTime * 2.0f));
	const float maxVelocity = Physics::PIXELS_PER_METRE * 2.0f;
	const float velocity = gameObject->GetComponent<RigidBody2D>().lock()->GetVelocity().Magnitude();
	const float velocityThreshold = (maxVelocity *0.25f);
	float newZoom = (velocity - velocityThreshold) / (maxVelocity - velocityThreshold);
	if(velocity > velocityThreshold) engine.GetGraphics()->SetCameraZoom(1.0f + newZoom * 0.25f);
}

void TestScene::Update(const float & deltaTime) {
	Scene::Update(deltaTime);
	Test(deltaTime);
}

void TestScene::Render() {
	Engine engine = Engine::GetInstance();
	Input * input = engine.GetInput();
	const float deltaTime = engine.GetTimer()->GetDeltaTime();
	std::shared_ptr<GameObject> gameObject = gameObjectManager->GetGameObject("Player").lock();
	
	//engine.GetGraphics()->Draw(circle, g1.lock()->GetComponent<Transform2D>().lock()->GetWorldTransform());
	//engine.GetGraphics()->Draw("Total Time = " + std::to_string(engine.GetTimer()->GetTotalTime()) + "	Delta Time = " + std::to_string(engine.GetTimer()->GetDeltaTime()) + "	FPS = " + std::to_string(engine.GetFPS()), Vector2(100.0f, 650.0f), 30);
	//engine.GetGraphics()->Draw("Mouse Position | X = " + std::to_string((int)mousePosition.x) + "  Y = " + std::to_string((int)mousePosition.y), Vector2(100.0f, 50.0f), 30);
	/*if(engine.GetInput()->GetKeyDown(KeyCodes::F)) {
		std::string toPrint = "";
		for(size_t i = 0; i < sf::Joystick::ButtonCount; i++) {
			toPrint += ("Button " + std::to_string(i) + " " + std::to_string(sf::Joystick::isButtonPressed(0, i)) + "\n");
		}
		std::cout << toPrint;
	}*/
	
	Scene::Render();
}

//void TestScene::SetUpShapes() {
//	sf::Vector2f rectSize(20, 20);
//	rect = sf::RectangleShape(rectSize);
//	sf::Color colour = sf::Color::Green;
//	rect.setOutlineColor(colour);
//	rect.setFillColor(colour);
//	rect.setOutlineThickness(1);
//	rect.setOrigin(sf::Vector2f(rectSize.x / 2, rectSize.y / 2));
//
//	const float radius = 20;
//	circle = sf::CircleShape(radius, 32);
//	colour = sf::Color::Blue;
//	circle.setOutlineColor(colour);
//	circle.setFillColor(colour);
//	circle.setOutlineThickness(1);
//	circle.setOrigin(radius, radius);
//}

void TestScene::Test(const float & deltaTime) {
	Engine engine = Engine::GetInstance();
	Input * input = engine.GetInput();
	mousePosition = input->GetMousePosition();

	//std::shared_ptr<GameObject> gameObject = gameObjectManager->GetGameObject("Player").lock();
	
	// Test Kinematic RigidBody Movement
	// gameObjectManager->GetGameObject("Background").lock()->GetComponent<Transform2D>().lock()->Move(Vector2(20.0f * deltaTime, 0.0f));

	// Input test
	//if(input->GetMouseButton(MouseButtons::MouseButton::Left) || input->GetMouseButton(MouseButtons::MouseButton::Middle) ||
	//   input->GetMouseButton(MouseButtons::MouseButton::Right) || input->GetMouseButton(MouseButtons::MouseButton::XButton1) ||
	//   input->GetMouseButton(MouseButtons::MouseButton::XButton2)) engine.GetGraphics()->Draw("Mouse Button Pressed", Vector2(1180, 100), 30, TextAlignment::RIGHT_ALIGNED);

	// Box2D test
	if(input->GetMouseButtonDown(MouseButtons::MouseButton::Right)) physicsSystem->CreateBox(mousePosition.x, mousePosition.y, Physics::PIXELS_PER_METRE, Physics::PIXELS_PER_METRE);

	
}