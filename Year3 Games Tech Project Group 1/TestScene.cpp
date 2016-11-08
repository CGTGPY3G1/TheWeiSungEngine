#include "TestScene.h"
#include "Engine.h"
#include "Input.h"
#include "Time.h"
#include "Graphics.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "PhysicsSystem.h"

sf::RectangleShape rect;
sf::CircleShape circle;
TestScene::TestScene() : Scene() {
}

TestScene::~TestScene() {
}

void TestScene::Start() {
	Scene::Start();
	SetUpShapes();
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
	r->Init(b2BodyType::b2_dynamicBody);
	std::shared_ptr<CircleCollider> c = t1->AddComponent<CircleCollider>().lock();
	c->Init(Vector2(), 20.0f);
	std::shared_ptr<RigidBody2D> r2 = t2->AddComponent<RigidBody2D>().lock();
	r2->Init(b2BodyType::b2_kinematicBody);
	std::shared_ptr<PolygonCollider> house = t2->AddComponent<PolygonCollider>().lock();
	house->Init(Vector2(350.0f, 150.0f), {Vector2(-50.0f, 150.0f), Vector2(50.0f, 150.0f), Vector2(50.0f, 50.0f), Vector2(0.0f, -50.0f), Vector2(-50.0f, 50.0f)}, true);
	for(size_t i = 0; i < 3; i++) {
		std::shared_ptr<BoxCollider> b = t2->AddComponent<BoxCollider>().lock();
		b->Init(Vector2(1000.0f, 384.0f * i), Vector2(512.0f, 256.0f));
	}
}

void TestScene::Update(const float & deltaTime) {
	Scene::Update(deltaTime);
	Test(deltaTime);
}

void TestScene::Render() {
	Engine & engine = Engine::GetInstance();
	engine.GetGraphics()->Draw(circle, g1.lock()->GetComponent<Transform2D>().lock()->GetWorldTransform());
	engine.GetGraphics()->Draw("Delta Time = " + std::to_string(engine.GetTimer()->GetDeltaTime()), Vector2(540.0f, 650.0f), 30);
	engine.GetGraphics()->Draw("FPS = " + std::to_string(engine.GetFPS()), Vector2(1180.0f, 650.0f), 30, RIGHT_ALIGNED);
	engine.GetGraphics()->Draw("Total Time = " + std::to_string(engine.GetTimer()->GetTotalTime()), Vector2(100.0f, 650.0f), 30);
	engine.GetGraphics()->Draw("Mouse Position | X = " + std::to_string((int)mousePosition.x) + "  Y = " + std::to_string((int)mousePosition.y), Vector2(100.0f, 50.0f), 30);
	Scene::Render();
}

void TestScene::SetUpShapes() {
	sf::Vector2f rectSize(20, 20);
	rect = sf::RectangleShape(rectSize);
	sf::Color colour = sf::Color::Green;
	rect.setOutlineColor(colour);
	rect.setFillColor(colour);
	rect.setOutlineThickness(1);
	rect.setOrigin(sf::Vector2f(rectSize.x / 2, rectSize.y / 2));

	const float radius = 20;
	circle = sf::CircleShape(radius, 32);
	colour = sf::Color::Blue;
	circle.setOutlineColor(colour);
	circle.setFillColor(colour);
	circle.setOutlineThickness(1);
	circle.setOrigin(radius, radius);
}

void TestScene::Test(const float & deltaTime) {
	Engine engine = Engine::GetInstance();
	Input * input = engine.GetInput();
	mousePosition = input->GetMousePosition();

	std::shared_ptr<GameObject> gameObject = gameObjectManager->GetGameObject("Player").lock();
	// Input test
	if(input->GetMouseButton(MouseButtons::MouseButton::Left) || input->GetMouseButton(MouseButtons::MouseButton::Middle) ||
	   input->GetMouseButton(MouseButtons::MouseButton::Right) || input->GetMouseButton(MouseButtons::MouseButton::XButton1) ||
	   input->GetMouseButton(MouseButtons::MouseButton::XButton2)) engine.GetGraphics()->Draw("Mouse Button Pressed", Vector2(1180, 100), 30, TextAlignment::RIGHT_ALIGNED);

	// Box2D test
	if(input->GetMouseButtonDown(MouseButtons::MouseButton::Right)) physicsSystem->CreateBox(mousePosition.x, mousePosition.y, Physics::PIXELS_PER_METRE, Physics::PIXELS_PER_METRE);

	// Movement Test
	float moveAmount = 256.0f;
	if(input->GetKey(KeyCodes::KeyCode::Up)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(0.0f, -moveAmount * deltaTime), ForceType::IMPULSE_FORCE);
	if(input->GetKey(KeyCodes::KeyCode::Down)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(0.0f, moveAmount * deltaTime), ForceType::IMPULSE_FORCE);
	if(input->GetKey(KeyCodes::KeyCode::Left)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(-moveAmount * deltaTime, 0.0f), ForceType::IMPULSE_FORCE);
	if(input->GetKey(KeyCodes::KeyCode::Right)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(moveAmount * deltaTime, 0.0f), ForceType::IMPULSE_FORCE);
	
	// Camera test
	if(input->GetKey(KeyCodes::KeyCode::W)) engine.GetGraphics()->MoveCamera(0.0f, -500.0f * deltaTime);
	if(input->GetKey(KeyCodes::KeyCode::S)) engine.GetGraphics()->MoveCamera(0.0f, 500.0f * deltaTime);
	if(input->GetKey(KeyCodes::KeyCode::A)) engine.GetGraphics()->MoveCamera(-500.0f * deltaTime, 0.0f);
	if(input->GetKey(KeyCodes::KeyCode::D)) engine.GetGraphics()->MoveCamera(500.0f * deltaTime, 0.0f);

	// Component Retrieval test
	if(input->GetKey(KeyCodes::KeyCode::T)) {
		const int NUMBER_OF_TIMES = 100000;
		//Profiler p = Profiler("Get " + std::to_string(NUMBER_OF_TIMES) + " Components");
		//p.StartProfiling();
		for(int i = 0; i < NUMBER_OF_TIMES; i++) {
			std::shared_ptr<Transform2D> t = g1.lock()->GetComponent<Transform2D>().lock();
		}
		//p.StopProfiling();
	}

	if(input->GetKey(KeyCodes::KeyCode::G)) {
		std::vector<std::weak_ptr<Transform2D>> c = g1.lock()->GetComponents<Transform2D>();
		const int SPACING = 30;
		int offsetY = ((SPACING * c.size()) / 2) - SPACING / 2;
		for(std::vector<std::weak_ptr<Transform2D>>::iterator i = c.begin(); i != c.end(); ++i, offsetY -= SPACING) {
			engine.GetGraphics()->Draw("Got ComponentType " + std::to_string((*i).lock()->Type()), Vector2(640.0f, 360.0f + offsetY), 30, CENTRE_ALIGNED);
		}
	}
}