
#include <iostream>
#include <thread>
#include "Engine.h"
#include "Graphics.h"
#include "Input.h"
#include "Time.h"
#include "Profiler.h"
#include "ComponentHeaders.h"
#include "GameObject.h"
#include <bitset>
#include "Scene.h"
#include <mutex>
#include "GameObjectManager.h"
#include "PhysicsSystem.h"
Engine & engine = Engine::GetInstance();
std::weak_ptr<GameObject> g1, g2;
std::mutex m;
float rotation = 10.0f;
sf::RectangleShape rect;
sf::CircleShape circle;
Vector2 mousePosition;

Scene * scene = new Scene();
Scene * scene2 = new Scene();
unsigned int fps = 0;
float WorldToScreenY(const float & y) {
	return engine.GetGraphics()->GetScreenResolution().y - y;
}

void SetUpShapes() {
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

void ThreadTest(const size_t & threadNumber, const size_t & numberOfCalls) {
	for(size_t i = 0; i < numberOfCalls; i++) {
		Vector2 mp = engine.GetInput()->GetMousePosition();
		//std::lock_guard<std::mutex> lock(m);
		//std::cout << std::to_string(threadNumber) + ": Call Number: " + std::to_string(i) + " : " << mp << std::endl;
	}
}

const float FIXED_TIME_STEP = 0.02f;
float accumulator = 0.0f;
void Test() {
	Input * input = engine.GetInput();
	input->Update();
	Time * timer = engine.GetTimer();
	timer->Update();
	mousePosition = input->GetMousePosition();
	float deltaTime = timer->GetDeltaTime();

	std::shared_ptr<GameObject> gameObject = g1.lock();		
	// Input test
	if(input->GetMouseButton(MouseButtons::MouseButton::Left) || input->GetMouseButton(MouseButtons::MouseButton::Middle) || 
	   input->GetMouseButton(MouseButtons::MouseButton::Right) || input->GetMouseButton(MouseButtons::MouseButton::XButton1) || 
	   input->GetMouseButton(MouseButtons::MouseButton::XButton2)) engine.GetGraphics()->Draw("Mouse Button Pressed", Vector2(1180, 100), 30, TextAlignment::RIGHT_ALIGNED);

	if(input->GetKeyDown(KeyCodes::KeyCode::N)) {
		const unsigned int NUMBER_OF_GETS = 50000;
		Profiler tp = Profiler("Input Test: Get Mouse Position " + std::to_string(NUMBER_OF_GETS) + " Times");
		tp.StartProfiling();
		for(size_t i = 0; i < NUMBER_OF_GETS; i++) {
			engine.GetInput()->GetMousePosition();
		}
		tp.StopProfiling();
	}

	// Box2D test
	scene->GetPhysicsSystem()->Update(deltaTime);
	if(input->GetMouseButtonDown(MouseButtons::MouseButton::Right)) scene->GetPhysicsSystem()->CreateBox(mousePosition.x, mousePosition.y, Physics::PIXELS_PER_METRE, Physics::PIXELS_PER_METRE);

	// Movement Test
	float moveAmount = 256.0f;
	if(input->GetKey(KeyCodes::KeyCode::Up)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(0.0f, -moveAmount * deltaTime), ForceType::IMPULSE_FORCE);
	if(input->GetKey(KeyCodes::KeyCode::Down)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(0.0f, moveAmount * deltaTime), ForceType::IMPULSE_FORCE);
	if(input->GetKey(KeyCodes::KeyCode::Left)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(-moveAmount * deltaTime, 0.0f), ForceType::IMPULSE_FORCE);
	if(input->GetKey(KeyCodes::KeyCode::Right)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(moveAmount * deltaTime, 0.0f), ForceType::IMPULSE_FORCE);
	engine.GetGraphics()->Draw(circle, gameObject->GetComponent<Transform2D>().lock()->GetWorldTransform());


	// Graphics test
	engine.GetGraphics()->Draw("Delta Time = " + std::to_string(deltaTime), Vector2(540.0f, 650.0f), 30);
	engine.GetGraphics()->Draw("FPS = " + std::to_string(fps), Vector2(1180.0f, 650.0f), 30, RIGHT_ALIGNED);
	engine.GetGraphics()->Draw("Total Time = " + std::to_string(timer->GetTotalTime()), Vector2(100.0f, 650.0f), 30);
	engine.GetGraphics()->Draw("Mouse Position | X = " + std::to_string((int)mousePosition.x) + "  Y = " + std::to_string((int)mousePosition.y), Vector2(100, 100), 30);

	// Camera test
	if(input->GetKey(KeyCodes::KeyCode::W)) engine.GetGraphics()->MoveCamera(0.0f, -500.0f * deltaTime);
	if(input->GetKey(KeyCodes::KeyCode::A)) engine.GetGraphics()->MoveCamera(0.0f, 500.0f * deltaTime);
	if(input->GetKey(KeyCodes::KeyCode::S)) engine.GetGraphics()->MoveCamera(-500.0f * deltaTime, 0.0f);
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
		std::list<std::weak_ptr<Transform2D>> c = g1.lock()->GetComponents<Transform2D>();
		const int SPACING = 30;
		int offsetY = ((SPACING * c.size()) / 2) - SPACING/2;
		for(std::list<std::weak_ptr<Transform2D>>::iterator i = c.begin(); i != c.end(); ++i, offsetY -= SPACING) {
			engine.GetGraphics()->Draw("Got ComponentType " + std::bitset<32>(((*i).lock()->Type())).to_string(), Vector2(640.0f, 360.0f + offsetY), 30, CENTRE_ALIGNED);
		}
	}
}

void CreateObjects() {
	std::shared_ptr<GameObject> u = (scene->GetGameObjectManager().lock())->CreateGameObject().lock();
	std::shared_ptr<GameObject> v = (scene->GetGameObjectManager().lock())->CreateGameObject().lock();
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
	std::shared_ptr<PolygonCollider> p = t2->AddComponent<PolygonCollider>().lock();
	p->Init(Vector2(350.0f, 150.0f), {Vector2(-50.0f, 50.0f), Vector2(0.0f, -50.0f), Vector2(50.0f, 50.0f)});
	for(size_t i = 0; i < 3; i++) {
		std::shared_ptr<BoxCollider> b = t2->AddComponent<BoxCollider>().lock();
		b->Init(Vector2(1000.0f, 384.0f * i), Vector2(512.0f, 256.0f));
	}
}

void main(int argA, char** argB) {
	GraphicsSettings gs;
	gs.screenPosition = Vector2(500, 200);
	gs.resolution = Vector2(1280, 720);
	gs.windowTitle = "Test";
	gs.resizeable = true;
	Graphics * graphics = engine.GetGraphics();
	graphics->SetSettings(gs);
	CreateObjects();
	float fpsTimer = 0.0f;
	unsigned int framesThisSecond = 0;
	SetUpShapes();
	while(graphics->GetWindowOpen()) {
		graphics->Clear();
		Test();
		float delta = engine.GetTimer()->GetDeltaTime();
		scene->Update(delta);
		graphics->Update();
		framesThisSecond++;
		fpsTimer += engine.GetTimer()->GetDeltaTime();
		if(fpsTimer > 1.0f) {
			fps = framesThisSecond; // SmoothFPS ((fps + framesThisSecond) / 2) * fpsTimer;
			framesThisSecond = 0;
			fpsTimer = 0.0f;
		}
	}
}

