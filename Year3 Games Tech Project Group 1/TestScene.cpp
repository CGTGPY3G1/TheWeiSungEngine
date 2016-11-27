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
	sprite->Init("Images/Buildings.png", PivotPoint::Centre, RenderLayer::FOREGROUND_LAYER, 5000);
	std::shared_ptr<RigidBody2D> r = building->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_kinematicBody);
	const float gridSize = 32.0f;
	switch(buildingNumber) {
	case 1:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(64.0f, 64.0f));
		sprite->SetTextureRect(std::roundl(gridSize * 3), std::roundl(gridSize * 5), std::roundl(gridSize * 2), std::roundl(gridSize * 2));
		break;
	case 2:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 6.0f, gridSize * 5.0f));
		sprite->SetTextureRect(0, 0, std::roundl(gridSize * 6), std::roundl(gridSize * 5));
		break;
	case 3:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-240.0f, 0.0f), Vector2(72.0f, 124.0f));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(50.0f, -300.0f), Vector2(168.0f, 74.0f));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(50.0f, 300.0f), Vector2(168.0f, 74.0f));
		sprite->SetTextureRect(std::roundl(gridSize * 6), 0, std::roundl(gridSize * 6), std::roundl(gridSize * 7));
		break;
	case 4:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4.0f, gridSize * 3.0f));
		sprite->SetTextureRect(std::roundl(gridSize * 8), std::roundl(gridSize * 7), std::roundl(gridSize * 4), std::roundl(gridSize * 3));
		break;
	case 5:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 7.0f, gridSize * 9.0f));
		sprite->SetTextureRect(std::roundl(gridSize * 12), 0, std::roundl(gridSize * 7), std::roundl(gridSize * 9));
		break;
	case 6:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4.0f, gridSize * 7.0f));
		sprite->SetTextureRect(std::roundl(gridSize * 19), 0, std::roundl(gridSize * 4), std::roundl(gridSize * 7));
		break;
	case 7:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 8.0f, gridSize * 4.0f));
		sprite->SetTextureRect(0, std::roundl(gridSize * 7), std::roundl(gridSize * 8), std::roundl(gridSize * 4));
		break;
	case 8:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, -20.0f), Vector2(gridSize * 3.0f + 14.0f, gridSize * 3.0f - 10.0f));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-270.0f, 0.0f), Vector2(gridSize * 0.75f, gridSize * 3));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(270.0f, 00.0f), Vector2(gridSize * 0.75f, gridSize * 3));
		sprite->SetTextureRect(std::roundl(gridSize * 12), std::roundl(gridSize * 9), std::roundl(gridSize * 5), std::roundl(gridSize * 3));
		break;
	case 9:
		//building->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(gridSize * 2.0f, gridSize * 1.5f), false);             // Is now a tunnel
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-Physics::PIXELS_PER_METRE * 2.5f, 0.0f), Vector2(gridSize * 3, gridSize * 3));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(Physics::PIXELS_PER_METRE * 2.5f, 0.0f), Vector2(gridSize * 3, gridSize * 3));
		sprite->SetTextureRect(std::roundl(gridSize * 17), std::roundl(gridSize * 9), std::roundl(gridSize * 8), std::roundl(gridSize * 3));
		break;
	case 10:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 3.0f, gridSize * 2.0f));
		sprite->SetTextureRect(0, std::roundl(gridSize * 5), std::roundl(gridSize * 3), std::roundl(gridSize * 2));
		break;
	case 11:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(gridSize * 2.0f + 26.0f, gridSize * 2.0f + 26.0f));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-gridSize * 4.8f, -gridSize * 4.8f), Vector2(18, 18));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-gridSize * 4.8f, gridSize * 4.8f), Vector2(18, 18));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(gridSize * 4.8f, gridSize * 4.8f), Vector2(18, 18));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(gridSize * 4.8f, -gridSize * 4.8f), Vector2(18, 18));
		sprite->SetTextureRect(std::roundl(gridSize * 8), std::roundl(gridSize * 10), std::roundl(gridSize * 3), std::roundl(gridSize * 3));
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
	player->Init(Vector2(700.0f, 0.0f));
	std::shared_ptr<Transform2D> t1 = player->GetComponent<Transform2D>().lock();
	std::shared_ptr<RigidBody2D> r = t1->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_dynamicBody, 0.5f, 1.0f);
	std::shared_ptr<CircleCollider> c = t1->AddComponent<CircleCollider>().lock();
	c->Init(Vector2(0.0f, 0.0f), 14.0f, false);
	std::shared_ptr<CircleCollider> playerSensor = t1->AddComponent<CircleCollider>().lock();
	playerSensor->Init(Vector2(), 200.0f, true);
	std::shared_ptr<PolygonCollider> body = t1->AddComponent<PolygonCollider>().lock();
	body->Init(Vector2(), {Vector2(-7.0f, -28.0f), Vector2(-14.0f, 0.0f), Vector2(-7.0f, 28.0f), Vector2(-5.0f, 29.0f), Vector2(1.0f, 28.0f), Vector2(8.0f, 0.0f), Vector2(1.0f, -28.0f), Vector2(-5.0f, -29.0f)});
	std::shared_ptr<SpriteRenderer> sr = player->AddComponent<SpriteRenderer>().lock();
	player->AddComponent<CharacterMovementScript>().lock()->Start();

	sr->Init("Images/Player.png", PivotPoint::Centre, RenderLayer::FOREGROUND_LAYER, 85);
	r->SetMass(50);

	std::shared_ptr<GameObject> car = gameObjectManager->CreateGameObject("Car").lock();
	car->Init();
	std::shared_ptr<Transform2D> carTransform = car->GetComponent<Transform2D>().lock();
	carTransform->SetPosition(Vector2(-600.0f, -350.0f));
	std::shared_ptr<RigidBody2D> carRB = car->AddComponent<RigidBody2D>().lock();
	carRB->Init(b2BodyType::b2_dynamicBody, 0.5f, 1.0f);
	car->AddComponent<BoxCollider>().lock()->Init(Vector2(30.0f, 0.0f), Vector2(140.0f, 80.0f), false, 100.0f);
	carRB->SetMass(1500.0f);
	
	std::shared_ptr<GameObject> leftWheel = gameObjectManager->CreateGameObject("LeftWheel").lock();
	leftWheel->Init();
	std::shared_ptr<Transform2D> leftWheelTransform = leftWheel->GetComponent<Transform2D>().lock();
	leftWheelTransform->SetPosition(carTransform->GetPosition() + Vector2(80.0f, -40.0f));
	std::shared_ptr<RigidBody2D> leftWheelRB = leftWheel->AddComponent<RigidBody2D>().lock();
	leftWheelRB->Init(b2BodyType::b2_dynamicBody, 0.5f, 1.0f);
	std::shared_ptr<RevoluteJoint> leftWheelJoint = leftWheel->AddComponent<RevoluteJoint>().lock();
	leftWheelJoint->Init(carRB, Vector2(-50.0f, 0.0f), Vector2::Right);
	leftWheel->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(10.0f, 5.0f), false, 5.0f);
	
	std::shared_ptr<GameObject> rightWheel = gameObjectManager->CreateGameObject("RighttWheel").lock();
	rightWheel->Init();
	std::shared_ptr<Transform2D> rightWheelTransform = rightWheel->GetComponent<Transform2D>().lock();
	rightWheelTransform->SetPosition(carTransform->GetPosition() + Vector2(80.0f, 40.0f));
	std::shared_ptr<RigidBody2D> rightWheelRB = rightWheel->AddComponent<RigidBody2D>().lock();
	rightWheelRB->Init(b2BodyType::b2_dynamicBody, 0.5f, 1.0f);
	std::shared_ptr<RevoluteJoint> rightWheelJoint = rightWheel->AddComponent<RevoluteJoint>().lock();
	rightWheelJoint->Init(carRB, Vector2(-50.0f, 0.0f), Vector2::Right);
	rightWheel->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(10.0f, 5.0f), false, 5.0f);

	std::shared_ptr<GameObject> backLeftWheel = gameObjectManager->CreateGameObject("BackLeftWheel").lock();
	backLeftWheel->Init();
	std::shared_ptr<Transform2D> backLeftWheelTransform = backLeftWheel->GetComponent<Transform2D>().lock();
	backLeftWheelTransform->SetPosition(carTransform->GetPosition() +Vector2(-20.0f, -40.0f));
	std::shared_ptr<RigidBody2D> backLeftWheelRB = backLeftWheel->AddComponent<RigidBody2D>().lock();
	backLeftWheelRB->Init(b2BodyType::b2_dynamicBody, 0.5f, 1.0f);
	std::shared_ptr<RevoluteJoint> backLeftWheelJoint = backLeftWheel->AddComponent<RevoluteJoint>().lock();
	backLeftWheelJoint->Init(carRB, Vector2(-50.0f, 0.0f), Vector2::Right);
	backLeftWheel->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(10.0f, 5.0f), false, 5.0f);

	std::shared_ptr<GameObject> backRightWheel = gameObjectManager->CreateGameObject("BackRightWheel").lock();
	backRightWheel->Init();
	std::shared_ptr<Transform2D> backRightWheelTransform = backRightWheel->GetComponent<Transform2D>().lock();
	backRightWheelTransform->SetPosition(carTransform->GetPosition() + Vector2(-20.0f, 40.0f));
	std::shared_ptr<RigidBody2D> backRightWheelRB = backRightWheel->AddComponent<RigidBody2D>().lock();
	backRightWheelRB->Init(b2BodyType::b2_dynamicBody, 0.5f, 1.0f);
	std::shared_ptr<RevoluteJoint> backRightWheelJoint = backRightWheel->AddComponent<RevoluteJoint>().lock();
	backRightWheelJoint->Init(carRB, Vector2(-50.0f,0.0f), Vector2::Right);
	backRightWheel->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(10.0f, 5.0f), false, 5.0f);
	
	Vector2 scale = Vector2(4.0f, 4.0f);
	std::shared_ptr<GameObject> background = gameObjectManager->CreateGameObject("Background").lock();
	background->Init();
	std::shared_ptr<Transform2D> t2 = background->AddComponent<Transform2D>().lock();
	std::shared_ptr<SpriteRenderer> spriteRenderer = t2->AddComponent<SpriteRenderer>().lock();
	spriteRenderer->Init("Images/Street.png", PivotPoint::Centre, RenderLayer::BACKGROUND_LAYER, 0);
	spriteRenderer->GetSprite().setScale(sf::Vector2f(scale));
	const float ppm = Physics::PIXELS_PER_METRE, offset = ppm / 2.0f;
	
	CreateBuilding(3, Vector2(ppm * -2 + offset, ppm * 8), scale, 90.0f).lock()->GetComponent<SpriteRenderer>().lock()->SetColour(0.8f, 0.8f, 0.8f, 1.0f);
	CreateBuilding(2, Vector2(ppm * -2 + offset, offset), scale);
	Vector2 size = Vector2(0.0f, 0.0f);
	for(size_t i = 0; i < 2; i++) {
		for(size_t j = 0; j < 3; j++) {
			size = CreateBuilding(1, Vector2(ppm * -5 + offset + size.x * i, ppm * -11 + offset + size.y * j), scale).lock()->GetComponent<SpriteRenderer>().lock()->GetSize() + Vector2(ppm, ppm);
		}
	}
	CreateBuilding(3, Vector2(ppm * 4.0f, ppm * -8.0f + offset), scale);
	CreateBuilding(3, Vector2(ppm * 29, 0), scale, 90.0f);
	CreateBuilding(5, Vector2(ppm * 14, ppm * 7), scale, 90.0f);
	CreateBuilding(6, Vector2(ppm * 15 + offset, ppm * -7 - offset), scale);
	CreateBuilding(10, Vector2(ppm * 18 + offset, ppm * -5 - offset), scale, 90.0f);
	CreateBuilding(10, Vector2(ppm * 18 + offset, ppm * -9 - offset), scale, 90.0f);
	CreateBuilding(1, Vector2(ppm * 27, ppm * -10 - offset), scale);
	CreateBuilding(1, Vector2(ppm * 27, ppm * 10 + offset), scale);
	CreateBuilding(10, Vector2(ppm * 30 + offset, ppm * -10 - offset), scale);
	CreateBuilding(10, Vector2(ppm * 30 + offset, ppm * 10 + offset), scale);
	CreateBuilding(1, Vector2(ppm * -13, ppm * 10 + offset), scale);
	CreateBuilding(1, Vector2(ppm * -16, ppm * 10 + offset), scale);
	CreateBuilding(6, Vector2(ppm * -19, ppm * 2 - offset), scale, 90.0f);
	CreateBuilding(11, Vector2(ppm * -13, ppm * 2 - offset), scale);
	CreateBuilding(10, Vector2(ppm * -14 + offset, ppm * -7 - offset), scale);
	CreateBuilding(4, Vector2(ppm * -14 + offset, ppm * -10), scale);
	CreateBuilding(2, Vector2(ppm * -30 + offset, ppm * -9), scale);
	CreateBuilding(8, Vector2(ppm * -23 - offset, ppm * -9), scale, -90.0f);
	CreateBuilding(10, Vector2(ppm * -26 + offset, ppm * 10 + offset), scale);
	CreateBuilding(4, Vector2(ppm * -31 + offset, ppm * 10), scale);
	CreateBuilding(9, Vector2(ppm * -31 + offset, ppm * 4), scale, 90.0f);

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
	std::shared_ptr<Input> input = engine.GetInput().lock();
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
	engine.GetGraphics().lock()->MoveCamera((gameObject->GetComponent<RigidBody2D>().lock()->GetPosition() - engine.GetGraphics().lock()->GetCameraPosition()) *  (fixedDeltaTime * 1.5f));
	const float maxVelocity = Physics::PIXELS_PER_METRE * 2.0f;
	const float speed = gameObject->GetComponent<RigidBody2D>().lock()->GetSpeed();
	float oldZoom = engine.GetGraphics().lock()->GetCameraZoom();
	float newZoom = (speed / maxVelocity) * 0.4f;
	engine.GetGraphics().lock()->SetCameraZoom(std::max<float>(1.0f, (oldZoom * (1.0f - fixedDeltaTime) + ((1.0f + newZoom) * fixedDeltaTime))));
	/*if(input->GetKey(KeyCodes::KeyCode::Space)) {
		std::shared_ptr<GameObject> child = gameObjectManager->GetGameObject("Child").lock();
		std::cout << child->GetComponent<Transform2D>().lock()->GetForward() << std::endl;
	}*/
	Scene::FixedUpdate(fixedDeltaTime);
}

void TestScene::Update(const float & deltaTime) {
	Scene::Update(deltaTime);
	std::shared_ptr<Input> input = Engine::GetInstance().GetInput().lock();
	if(input->GetKeyDown(KeyCodes::KeyCode::C)) drawColliders = !drawColliders;
	if(input->GetKeyDown(KeyCodes::KeyCode::G))input->SetControllerActive(0, !input->GetControllerActive(0));
	if(input->GetKeyDown(KeyCodes::KeyCode::I)) oldInputStyle = !oldInputStyle;
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
	std::shared_ptr<Input> input = engine.GetInput().lock();
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