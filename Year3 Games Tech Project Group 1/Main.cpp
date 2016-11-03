
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
//GameObjectManager manager;
Scene * scene = new Scene();
//PhysicsSystem ps;
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
	//circle.setPosition(0, 0);
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
	// sf::Transform matrix stacking test
	std::shared_ptr<GameObject> gameObject = g1.lock(), gameObject2 = g2.lock();
	//gameObject->GetComponent<Transform2D>().lock()->SetPosition(mousePosition);
	//gameObject->GetComponent<Transform2D>().lock()->Rotate(180.0f * timer->GetDeltaTime());
	//gameObject2->GetComponent<Transform2D>().lock()->Rotate(180.0f * timer->GetDeltaTime());
	//engine.GetGraphics()->Draw(rect, gameObject->GetComponent<Transform2D>().lock()->GetWorldTransform());
	
	
	

	// Input test
	if(input->GetKeyDown(KeyCodes::KeyCode::A)) std::cout << "A Key Pressed" << std::endl;
	else if(input->GetKey(KeyCodes::KeyCode::A)) std::cout << "A Key Held" << std::endl;
	else if(input->GetKeyUp(KeyCodes::KeyCode::A)) std::cout << "A Key Released" << std::endl;
	if(input->GetKey(KeyCodes::KeyCode::D)) std::cout << "Delta Time = " << deltaTime << std::endl;
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

	// Threading Test
	if(input->GetKey(KeyCodes::KeyCode::M)) {		
		const size_t NUMBER_OF_CALLS = 20000, NUMBER_OF_THREADS = 8;
		std::thread t [NUMBER_OF_THREADS];
		for(size_t i = 0; i < NUMBER_OF_THREADS; i++) t[i] = std::thread(ThreadTest, i+1, NUMBER_OF_CALLS);
		for(size_t i = 0; i < NUMBER_OF_THREADS; i++) t[i].join();
	}

	// Box2D test
	scene->GetPhysicsSystem()->Update(deltaTime);
	if(input->GetMouseButtonDown(MouseButtons::MouseButton::Right)) scene->GetPhysicsSystem()->CreateBox(mousePosition.x, mousePosition.y, Physics::PIXELS_PER_METRE, Physics::PIXELS_PER_METRE);
	//if(input->GetKeyDown(KeyCodes::KeyCode::Up)) scene->GetPhysicsSystem()->SetGravity(0.0f, -10.0f);
	//if(input->GetKeyDown(KeyCodes::KeyCode::Down)) scene->GetPhysicsSystem()->SetGravity(0.0f, 10.0f);
	//if(input->GetKeyDown(KeyCodes::KeyCode::Space)) scene->GetPhysicsSystem()->SetGravity(0.0f, 0.0f);

	// Movement Test
	float moveAmount = 256.0f;
	if(input->GetKey(KeyCodes::KeyCode::Up)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(0.0f, -moveAmount * deltaTime), ForceType::IMPULSE_FORCE);
	if(input->GetKey(KeyCodes::KeyCode::Down)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(0.0f, moveAmount * deltaTime), ForceType::IMPULSE_FORCE);
	if(input->GetKey(KeyCodes::KeyCode::Left)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(-moveAmount * deltaTime, 0.0f), ForceType::IMPULSE_FORCE);
	if(input->GetKey(KeyCodes::KeyCode::Right)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(moveAmount * deltaTime, 0.0f), ForceType::IMPULSE_FORCE);
	engine.GetGraphics()->Draw(circle, gameObject->GetComponent<Transform2D>().lock()->GetWorldTransform());


	// Graphics test
	//engine.GetGraphics()->DrawCircle(Vector2(mousePosition.x, mousePosition.y), 5.0f, true);
	//engine.GetGraphics()->DrawCircle(Vector2(640, 360), 50, false, 1.0f, 0.0f, 0.0f);
	//rotation += deltaTime * 180.0f;
	//engine.GetGraphics()->DrawRect(Vector2(320.0f, 360.0f), Vector2(40.0f, 30.0f), rotation);
	engine.GetGraphics()->Draw("Delta Time = " + std::to_string(deltaTime), Vector2(540.0f, 650.0f), 30);
	engine.GetGraphics()->Draw("FPS = " + std::to_string(fps), Vector2(1180.0f, 650.0f), 30, RIGHT_ALIGNED);
	engine.GetGraphics()->Draw("Total Time = " + std::to_string(timer->GetTotalTime()), Vector2(100.0f, 650.0f), 30);
	engine.GetGraphics()->Draw("Mouse Position | X = " + std::to_string((int)mousePosition.x) + "  Y = " + std::to_string((int)mousePosition.y), Vector2(100, 100), 30);

	// Camera test
	/*if(input->GetKey(KeyCodes::KeyCode::Up)) engine.GetGraphics()->MoveCamera(0.0f, -500.0f * deltaTime);
	if(input->GetKey(KeyCodes::KeyCode::Down)) engine.GetGraphics()->MoveCamera(0.0f, 500.0f * deltaTime);
	if(input->GetKey(KeyCodes::KeyCode::Left)) engine.GetGraphics()->MoveCamera(-500.0f * deltaTime, 0.0f);
	if(input->GetKey(KeyCodes::KeyCode::Right)) engine.GetGraphics()->MoveCamera(500.0f * deltaTime, 0.0f);*/

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

void StackTransforms() {
	std::shared_ptr<GameObject> u = (scene->GetGameObjectManager().lock())->CreateGameObject().lock();
	std::shared_ptr<GameObject> v = (scene->GetGameObjectManager().lock())->CreateGameObject().lock();
	u->Init();
	v->Init();
	g1 = u;
	g2 = v;
	std::shared_ptr<Transform2D> t1 = u->AddComponent<Transform2D>().lock(); 
	std::shared_ptr<Transform2D> t2 = v->AddComponent<Transform2D>().lock();
	std::cout << std::to_string(t1.use_count()) << std::endl;
	std::cout << std::to_string(t2.use_count()) << std::endl;
	/*t1->Translate({500, 400});
	t2->Rotate(45.0f);
	t2->Translate(Vector2(100.0f, 0.0f));
	t2->Rotate(180.0f);*/
	//t2->Translate(Vector2(150.0f, 150.0f));
	//t2->SetParent(t1);
	/*std::shared_ptr<RigidBody2D> r = u->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_kinematicBody);
	std::shared_ptr<BoxCollider> b = u->AddComponent<BoxCollider>().lock();
	b->Init(Vector2(), Vector2(20.0f, 20.0f));*/

	t1->SetPosition(Vector2(640.0f, 360.0f));
	std::shared_ptr<RigidBody2D> r = t1->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_dynamicBody);
	std::shared_ptr<CircleCollider> c = t1->AddComponent<CircleCollider>().lock();
	c->Init(Vector2(), 20.0f);
	

	t2->SetPosition(Vector2(200.0f, 360.0f));
	std::shared_ptr<RigidBody2D> r2 = t2->AddComponent<RigidBody2D>().lock();
	std::cout << std::to_string(r2.use_count()) << std::endl;
	r2->Init(b2BodyType::b2_kinematicBody);
	std::shared_ptr<PolygonCollider> p = t2->AddComponent<PolygonCollider>().lock();
	p->Init(Vector2(0,0), {Vector2(-50, 50), Vector2(0, -50), Vector2(50, 50)});
	std::shared_ptr<BoxCollider> b = t2->AddComponent<BoxCollider>().lock();
	b->Init(Vector2(300, 0.0f), Vector2(64.0f, 64.0f));
}

void main(int argA, char** argB) {
	GraphicsSettings gs;
	gs.screenPosition = Vector2(500, 200);
	gs.resolution = Vector2(1280, 720);
	gs.windowTitle = "Test";
	gs.resizeable = true;
	Graphics * graphics = engine.GetGraphics();
	graphics->SetSettings(gs);
	StackTransforms();
	float fpsTimer = 0;
	unsigned int framesThisSecond = 0;
	SetUpShapes();
	while(graphics->GetWindowOpen()) {
		graphics->Clear();
		Test();
		scene->GetPhysicsSystem()->Draw();
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

