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

TestScene::TestScene() : Scene() {
}

TestScene::~TestScene() {
}

std::weak_ptr<GameObject> TestScene::CreateBuilding(const int & buildingNumber, const Vector2 & position, const Vector2 & scale, const float & rotation) {
	if(buildingNumber < 1 || buildingNumber > 3) std::weak_ptr<GameObject>();
	std::shared_ptr<GameObject> building = gameObjectManager->CreateGameObject("Building").lock();
	building->Init(position, rotation, scale);
	std::shared_ptr<SpriteRenderer> sprite = building->AddComponent<SpriteRenderer>().lock();
	sprite->Init("Images/Buildings.png", RenderLayer::MIDGROUND_LAYER);
	std::shared_ptr<RigidBody2D> r = building->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_kinematicBody);
	const float gridSize = 32.0f;
	switch(buildingNumber) {
	case 1:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(64.0f, 64.0f), false);
		sprite->SetTextureRect(gridSize * 3, gridSize * 5, gridSize * 2, gridSize * 2);
		break;
	case 2:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 6, gridSize * 5), false);
		sprite->SetTextureRect(0, 0, gridSize * 6, gridSize * 5);
		break;
	case 3:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-240.0, 0.0f), Vector2(72.0f, 124.0f), false);
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(50, -300.0f), Vector2(168.0f, 74.0f), false);
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(50, 300.0f), Vector2(168.0f, 74.0f), false);
		sprite->SetTextureRect(gridSize * 6, 0, gridSize * 6, gridSize * 7);
		break;
	case 4:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4, gridSize * 3), false);
		sprite->SetTextureRect(gridSize * 8, gridSize * 7, gridSize * 4, gridSize * 3);
		break;
	case 5:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 7, gridSize * 9), false);
		sprite->SetTextureRect(gridSize * 12, 0, gridSize * 7, gridSize * 9);
		break;
	case 6:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4, gridSize * 7), false);
		sprite->SetTextureRect(gridSize * 19, 0, gridSize * 4, gridSize * 7);
		break;
	case 7:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 8, gridSize * 4), false);
		sprite->SetTextureRect(0, gridSize * 7, gridSize * 8, gridSize * 4);
		break;
	case 8:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, -20.0f), Vector2(gridSize * 3 + 14, gridSize * 3 - 10), false);
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-270.0f, 0.0f), Vector2(gridSize * 0.75f, gridSize * 3), false);
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(270.0f, 00.0f), Vector2(gridSize * 0.75f, gridSize * 3), false);
		sprite->SetTextureRect(gridSize * 12, gridSize * 9, gridSize * 5, gridSize * 3);
		break;
	case 9:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(gridSize * 2.0f, gridSize * 1.5f), false);
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-Physics::PIXELS_PER_METRE * 2.5f, 0.0f), Vector2(gridSize * 3, gridSize * 3), false);
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(Physics::PIXELS_PER_METRE * 2.5f, 0.0f), Vector2(gridSize * 3, gridSize * 3), false);
		sprite->SetTextureRect(gridSize * 17, gridSize * 9, gridSize * 8, gridSize * 3);
		break;
	case 10:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 3, gridSize * 2), false);
		sprite->SetTextureRect(0, gridSize * 5, gridSize * 3, gridSize * 2);
		break;
	case 11:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(gridSize * 2.0f + 26.0f, gridSize * 2.0f + 26.0f), false);
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-gridSize * 4.8f, -gridSize * 4.8f), Vector2(18, 18), false);
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-gridSize * 4.8f, gridSize * 4.8f), Vector2(18, 18), false);
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(gridSize * 4.8f, gridSize * 4.8f), Vector2(18, 18), false);
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(gridSize * 4.8f, -gridSize * 4.8f), Vector2(18, 18), false);
		sprite->SetTextureRect(gridSize * 8, gridSize * 10, gridSize * 3, gridSize * 3);
		break;
	default:
		break;
	}
	return building;
}

void TestScene::Start() {
	Scene::Start();
	//Engine::GetInstance().GetInput()->SetControllerActive(0, true);
	std::shared_ptr<GameObject> player = gameObjectManager->CreateGameObject("Player").lock();
	player->Init(Vector2(50.0f, -400.0f));
	std::shared_ptr<Transform2D> t1 = player->GetComponent<Transform2D>().lock();
	std::shared_ptr<RigidBody2D> r = t1->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_dynamicBody, 1.0f, 0.5f, 1.0f);
	r->SetMass(50);
	std::shared_ptr<CircleCollider> c = t1->AddComponent<CircleCollider>().lock();
	c->Init(Vector2(0.0f, 0.0f), 14.0f, false);
	std::shared_ptr<CircleCollider> playerSensor = t1->AddComponent<CircleCollider>().lock();
	playerSensor->Init(Vector2(), 200.0f, true);
	std::shared_ptr<PolygonCollider> body = t1->AddComponent<PolygonCollider>().lock();
	body->Init(Vector2(), {Vector2(-7, -28), Vector2(-14, 0), Vector2(-7, 28), Vector2(-5, 29), Vector2(1, 28), Vector2(8, 0), Vector2(1, -28), Vector2(-5, -29)});
	std::shared_ptr<SpriteRenderer> sr = player->AddComponent<SpriteRenderer>().lock();
	sr->Init("Images/Player.png", RenderLayer::FOREGROUND_LAYER, 85);
	//std::shared_ptr<GameObject> child = gameObjectManager->CreateGameObject("Child").lock();
	//child->Init();
	//std::shared_ptr<Transform2D> childTransform = child->GetComponent<Transform2D>().lock();	
	//childTransform->Rotate(90.0f);
	//childTransform->SetParent(t1);
	//childTransform->SetPosition(Vector2(200.0f, 200.0f));
	//childTransform->SetScale(Vector2(2.0f, 2.0f));
	//std::shared_ptr<SpriteRenderer> childSprite = childTransform->AddComponent<SpriteRenderer>().lock();
	//childSprite->Init("Images/Ball.png", RenderLayer::FOREGROUND_LAYER, 85);
	//std::shared_ptr<RigidBody2D> chlldRB = child->AddComponent<RigidBody2D>().lock();
	//chlldRB->Init(b2BodyType::b2_kinematicBody, 1.0f, 0.5f, 1.0f);
	//std::shared_ptr<BoxCollider> c2 = child->AddComponent<BoxCollider>().lock();
	//c2->Init(Vector2(), Vector2(40.0f, 40.0f), false);

	std::shared_ptr<GameObject> background = gameObjectManager->CreateGameObject("Background").lock();
	background->Init();
	std::shared_ptr<Transform2D> t2 = background->AddComponent<Transform2D>().lock();
	std::shared_ptr<SpriteRenderer> spriteRenderer = t2->AddComponent<SpriteRenderer>().lock();
	spriteRenderer->Init("Images/Street.png", RenderLayer::BACKGROUND_LAYER, 0);
	spriteRenderer->GetSprite().setScale(sf::Vector2f(4, 4));
	const float ppm = Physics::PIXELS_PER_METRE, offset = ppm / 2.0f;

	CreateBuilding(3, Vector2(ppm * -2 + offset, ppm * 8), Vector2(4, 4), 90.0f).lock()->GetComponent<SpriteRenderer>().lock()->SetColour(0.8f, 0.8f, 0.8f, 1.0f);
	CreateBuilding(2, Vector2(ppm * -2 + offset, offset), Vector2(4, 4));
	Vector2 size = Vector2(0.0f, 0.0f);
	for(size_t i = 0; i < 2; i++) {
		for(size_t j = 0; j < 3; j++) {
			size = CreateBuilding(1, Vector2(ppm * -5 + offset + size.x * i, ppm * -11 + offset + size.y * j), Vector2(4, 4)).lock()->GetComponent<SpriteRenderer>().lock()->GetSize() + Vector2(ppm, ppm);
		}
	}
	CreateBuilding(3, Vector2(ppm * 4, ppm * -8 + offset), Vector2(4, 4));
	CreateBuilding(3, Vector2(ppm * 29, 0), Vector2(4, 4), 90.0f);
	CreateBuilding(5, Vector2(ppm * 14, ppm * 7), Vector2(4, 4), 90.0f);
	CreateBuilding(6, Vector2(ppm * 15 + offset, ppm * -7 - offset), Vector2(4, 4));
	CreateBuilding(10, Vector2(ppm * 18 + offset, ppm * -5 - offset), Vector2(4, 4), 90.0f);
	CreateBuilding(10, Vector2(ppm * 18 + offset, ppm * -9 - offset), Vector2(4, 4), 90.0f);
	CreateBuilding(1, Vector2(ppm * 27, ppm * -10 - offset), Vector2(4, 4));
	CreateBuilding(1, Vector2(ppm * 27, ppm * 10 + offset), Vector2(4, 4));
	CreateBuilding(10, Vector2(ppm * 30 + offset, ppm * -10 - offset), Vector2(4, 4));
	CreateBuilding(10, Vector2(ppm * 30 + offset, ppm * 10 + offset), Vector2(4, 4));
	CreateBuilding(1, Vector2(ppm * -13, ppm * 10 + offset), Vector2(4, 4));
	CreateBuilding(1, Vector2(ppm * -16, ppm * 10 + offset), Vector2(4, 4));
	CreateBuilding(6, Vector2(ppm * -19, ppm * 2 - offset), Vector2(4, 4), 90.0f);
	CreateBuilding(11, Vector2(ppm * -13, ppm * 2 - offset), Vector2(4, 4));
	CreateBuilding(10, Vector2(ppm * -14 + offset, ppm * -7 - offset), Vector2(4, 4));
	CreateBuilding(4, Vector2(ppm * -14 + offset, ppm * -10), Vector2(4, 4));
	CreateBuilding(2, Vector2(ppm * -30 + offset, ppm * -9), Vector2(4, 4));
	CreateBuilding(8, Vector2(ppm * -23 - offset, ppm * -9), Vector2(4, 4), -90.0f);
	CreateBuilding(10, Vector2(ppm * -26 + offset, ppm * 10 + offset), Vector2(4, 4));
	CreateBuilding(4, Vector2(ppm * -31 + offset, ppm * 10), Vector2(4, 4));
	CreateBuilding(9, Vector2(ppm * -31 + offset, ppm * 4), Vector2(4, 4), 90.0f);

	std::shared_ptr<GameObject> boundaries = gameObjectManager->CreateGameObject("Boundaries").lock();
	boundaries->Init();
	std::shared_ptr<RigidBody2D> rb = boundaries->AddComponent<RigidBody2D>().lock();
	rb->Init(b2BodyType::b2_kinematicBody);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(0, -3008), Vector2(11520, ppm * 5), false);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(-5696, 8), Vector2(ppm * 5, 6016), false);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(0, 3008), Vector2(11520, ppm * 5), false);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(5696, 0), Vector2(ppm * 5, 6016), false);
}

void TestScene::FixedUpdate(const float & fixedDeltaTime) {
	Engine engine = Engine::GetInstance();
	Input * input = engine.GetInput();
	std::shared_ptr<GameObject> gameObject = gameObjectManager->GetGameObject("Player").lock();
	bool useController = input->GetControllerActive(0);

	// Movement Test
	Vector2 forward = gameObject->GetComponent<RigidBody2D>().lock()->GetForward();
	const float forwardAngle = forward.AngleInDegrees();
	float moveAmount = gameObject->GetComponent<RigidBody2D>().lock()->GetMass() * Physics::PIXELS_PER_METRE * 2.0f;
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
		if(input->GetKey(KeyCodes::KeyCode::Up) || input->GetKey(KeyCodes::KeyCode::W)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(0.0f, -moveAmount * ((std::max<float>(forward.Dot(Vector2(0, -1)), threshold)) + 1.0f)), ForceType::FORCE);
		if(input->GetKey(KeyCodes::KeyCode::Down) || input->GetKey(KeyCodes::KeyCode::S)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(0.0f, moveAmount * ((std::max<float>(forward.Dot(Vector2(0, 1)), threshold)) + 1.0f)), ForceType::FORCE);
		if(input->GetKey(KeyCodes::KeyCode::Left) || input->GetKey(KeyCodes::KeyCode::A)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(-moveAmount * ((std::max<float>(forward.Dot(Vector2(-1, 0)), threshold)) + 1.0f), 0.0f), ForceType::FORCE);
		if(input->GetKey(KeyCodes::KeyCode::Right) || input->GetKey(KeyCodes::KeyCode::D)) gameObject->GetComponent<RigidBody2D>().lock()->AddForce(Vector2(moveAmount * ((std::max<float>(forward.Dot(Vector2(1, 0)), threshold)) + 1.0f), 0.0f), ForceType::FORCE);
	}

	engine.GetGraphics()->MoveCamera((gameObject->GetComponent<RigidBody2D>().lock()->GetPosition() - engine.GetGraphics()->GetCameraPosition()) *  (fixedDeltaTime * 1.5f));
	const float maxVelocity = Physics::PIXELS_PER_METRE * 2.0f;
	const float speed = gameObject->GetComponent<RigidBody2D>().lock()->GetSpeed();
	float oldZoom = engine.GetGraphics()->GetCameraZoom();
	float newZoom = (speed / maxVelocity) * 0.4f;
	engine.GetGraphics()->SetCameraZoom(std::max<float>(1.0f, (oldZoom * (1.0f - fixedDeltaTime) + ((1.0f + newZoom) * fixedDeltaTime))));
	/*if(input->GetKey(KeyCodes::KeyCode::Space)) {
		std::shared_ptr<GameObject> child = gameObjectManager->GetGameObject("Child").lock();
		std::cout << child->GetComponent<Transform2D>().lock()->GetForward() << std::endl;
	}*/
	Scene::FixedUpdate(fixedDeltaTime);
}

void TestScene::Update(const float & deltaTime) {
	Scene::Update(deltaTime);
	Input * input = Engine::GetInstance().GetInput();
	if(input->GetKeyDown(KeyCodes::KeyCode::C)) drawColliders = !drawColliders;
	if(input->GetKeyDown(KeyCodes::KeyCode::G))input->SetControllerActive(0, !input->GetControllerActive(0));
	
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