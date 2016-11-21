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
	std::shared_ptr<GameObject> player = gameObjectManager->CreateGameObject("Player").lock();
	player->Init();
	std::shared_ptr<Transform2D> t1 = player->GetComponent<Transform2D>().lock();
	t1->SetPosition(Vector2(100.0f, 100.0f));
	std::shared_ptr<RigidBody2D> r = t1->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_dynamicBody, 1.0f, 0.5f, 1.0f);
	r->SetMass(50);
	std::shared_ptr<CircleCollider> c = t1->AddComponent<CircleCollider>().lock();
	c->Init(Vector2(), 20.0f, false);
	std::shared_ptr<CircleCollider> playerSensor = t1->AddComponent<CircleCollider>().lock();
	playerSensor->Init(Vector2(), 200.0f, true);
	std::shared_ptr<SpriteRenderer> sr = player->AddComponent<SpriteRenderer>().lock();
	sr->Init("Images/Ball.png", RenderLayer::FOREGROUND_LAYER, 85);

	/*std::shared_ptr<GameObject> child = gameObjectManager->CreateGameObject("Child").lock();
	child->Init();
	std::shared_ptr<Transform2D> childTransform = child->GetComponent<Transform2D>().lock();	
	childTransform->Rotate(90.0f);
	childTransform->SetParent(t1);
	childTransform->SetPosition(Vector2(200.0f, 200.0f));
	std::shared_ptr<SpriteRenderer> childSprite = childTransform->AddComponent<SpriteRenderer>().lock();
	childSprite->Init("Images/Ball.png", RenderLayer::FOREGROUND_LAYER, 85);
	std::shared_ptr<RigidBody2D> chlldRB = child->AddComponent<RigidBody2D>().lock();
	chlldRB->Init(b2BodyType::b2_kinematicBody, 1.0f, 0.5f, 1.0f);
	std::shared_ptr<BoxCollider> c2 = child->AddComponent<BoxCollider>().lock();
	c2->Init(Vector2(), Vector2(40.0f, 40.0f), false);*/

	std::shared_ptr<GameObject> background = gameObjectManager->CreateGameObject("Background").lock();
	background->Init();
	std::shared_ptr<Transform2D> t2 = background->AddComponent<Transform2D>().lock();
	t2->SetPosition(Vector2(640.0f, 360.0f));
	std::shared_ptr<SpriteRenderer> spriteRenderer = t2->AddComponent<SpriteRenderer>().lock();
	spriteRenderer->Init("Images/Street.png", RenderLayer::BACKGROUND_LAYER, 0);
	spriteRenderer->GetSprite().setScale(sf::Vector2f(4, 4));
}

void TestScene::FixedUpdate(const float & fixedDeltaTime) {
	Engine engine = Engine::GetInstance();
	Input * input = engine.GetInput();
	std::shared_ptr<GameObject> gameObject = gameObjectManager->GetGameObject("Player").lock();
	bool useController = false; // input->IsControllerConnected(0);

	// Movement Test

	Vector2 forward = Vector2(1, 0).RotateInDegrees(gameObject->GetComponent<RigidBody2D>().lock()->GetRotation());
	float moveAmount = gameObject->GetComponent<RigidBody2D>().lock()->GetMass() * Physics::PIXELS_PER_METRE * 2.0f;
	if(input->GetKey(KeyCodes::KeyCode::LShift)) moveAmount *= 1.5f;
	const float threshold = 0.25f;
	if(useController) {
		Vector2 directionToMouse = Vector2(input->GetAxis(0, ControllerButtons::ControllerAxis::RightStickH), input->GetAxis(0, ControllerButtons::ControllerAxis::RightStickV));
		if(directionToMouse.SquareMagnitude() > 0.5f) gameObject->GetComponent<RigidBody2D>().lock()->SetRotation(directionToMouse.AngleInDegrees());
		Vector2 moveDirection = Vector2(input->GetAxis(0, ControllerButtons::ControllerAxis::LeftStickH), input->GetAxis(0, ControllerButtons::ControllerAxis::LeftStickV));
		gameObject->GetComponent<RigidBody2D>().lock()->AddForce(moveDirection * moveAmount * ((std::max<float>(forward.Dot(moveDirection.Normalized()), threshold)) + 1.0f), ForceType::FORCE);
	}
	else {
		Vector2 directionToMouse = (mousePosition - gameObject->GetComponent<RigidBody2D>().lock()->GetPosition());
		gameObject->GetComponent<RigidBody2D>().lock()->SetRotation(directionToMouse.AngleInDegrees());
		if(input->GetKey(KeyCodes::KeyCode::Up) || input->GetKey(KeyCodes::KeyCode::W)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(0.0f, -moveAmount * ((std::max<float>(forward.Dot(Vector2(0, -1)), threshold)) + 1.0f)), ForceType::FORCE);
		if(input->GetKey(KeyCodes::KeyCode::Down) || input->GetKey(KeyCodes::KeyCode::S)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(0.0f, moveAmount * ((std::max<float>(forward.Dot(Vector2(0, 1)), threshold)) + 1.0f)), ForceType::FORCE);
		if(input->GetKey(KeyCodes::KeyCode::Left) || input->GetKey(KeyCodes::KeyCode::A)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(-moveAmount * ((std::max<float>(forward.Dot(Vector2(-1, 0)), threshold)) + 1.0f), 0.0f), ForceType::FORCE);
		if(input->GetKey(KeyCodes::KeyCode::Right) || input->GetKey(KeyCodes::KeyCode::D)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(moveAmount * ((std::max<float>(forward.Dot(Vector2(1, 0)), threshold)) + 1.0f), 0.0f), ForceType::FORCE);
	}

	Scene::FixedUpdate(fixedDeltaTime);
	engine.GetGraphics()->MoveCamera((gameObject->GetComponent<RigidBody2D>().lock()->GetPosition() - engine.GetGraphics()->GetCameraPosition()) *  (fixedDeltaTime * 1.5f));
	const float maxVelocity = Physics::PIXELS_PER_METRE * 2.0f;
	const float velocity = gameObject->GetComponent<RigidBody2D>().lock()->GetVelocity().Magnitude();
	const float velocityThreshold = (maxVelocity *0.25f);
	float newZoom = (velocity - velocityThreshold) / (maxVelocity - velocityThreshold);
	if(velocity > velocityThreshold) engine.GetGraphics()->SetCameraZoom(1.0f + newZoom * 0.25f);
	if(input->GetKey(KeyCodes::KeyCode::Space)) {
		std::shared_ptr<GameObject> child = gameObjectManager->GetGameObject("Child").lock();
		std::cout << child->GetComponent<Transform2D>().lock()->GetForward() << std::endl;
	}

}

void TestScene::Update(const float & deltaTime) {
	Scene::Update(deltaTime);
	Test(deltaTime);
}

void TestScene::Render() {
	//Engine engine = Engine::GetInstance();
	//Input * input = engine.GetInput();
	//const float deltaTime = engine.GetTimer()->GetDeltaTime();
	//std::shared_ptr<GameObject> gameObject = gameObjectManager->GetGameObject("Player").lock();
	
	//engine.GetGraphics()->Draw(circle, g1.lock()->GetComponent<Transform2D>().lock()->GetWorldTransform());
	//engine.GetGraphics()->Draw("Total Time = " + std::to_string(engine.GetTimer()->GetTotalTime()) + "	Delta Time = " + std::to_string(engine.GetTimer()->GetDeltaTime()) + "	FPS = " + std::to_string(engine.GetFPS()), Vector2(100.0f, 650.0f), 30);
	//engine.GetGraphics()->Draw("Mouse Position | X = " + std::to_string((int)mousePosition.x) + "  Y = " + std::to_string((int)mousePosition.y), Vector2(100.0f, 50.0f), 30);
	//std::string toPrint = "";
	//for(size_t i = 0; i < sf::Joystick::ButtonCount; i++) {
	//	if(sf::Joystick::isButtonPressed(0, i)) toPrint += ("Button " + std::to_string(i) + " is Pressed\n");
	//}
	//for(size_t i = 0; i < 11; i++) {
	//	float val = sf::Joystick::getAxisPosition(0, (sf::Joystick::Axis)i) *0.01f;
	//	if((val > 0.5f || val < -0.5f)) toPrint += ("Axis " + std::to_string(i) + " = "+ std::to_string(val) +"\n");
	//}
	/*for(size_t i = 0; i < ControllerButtons::ControllerAxis::AxisCount; i++) {
		float val = input->GetAxis(0, (ControllerButtons::ControllerAxis)i);
		if(val != 0) toPrint += ("Axis " + std::to_string(i) + " = " + std::to_string(val) + "\n");
	}
	std::cout << toPrint;*/
	
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