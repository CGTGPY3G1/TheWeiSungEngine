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
TestScene::TestScene() : Scene() {
}

TestScene::~TestScene() {
}

std::weak_ptr<GameObject> TestScene::CreateCharacter(const std::string & name, const int & characterType, const Vector2 & position, const Vector2 & scale, const float & rotation) {
	std::shared_ptr<GameObject> character = gameObjectManager->CreateGameObject(name).lock();
	character->Init(position, rotation, scale);
	std::shared_ptr<RigidBody2D> r = character->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_dynamicBody, 0.5f, 1.0f);
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
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-60.0f * scale.x, 0.0f), Vector2(72.0f, 124.0f));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(12.5f * scale.x, -75.0f * scale.y), Vector2(168.0f, 74.0f));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(12.5f * scale.x, 75.0f * scale.y), Vector2(168.0f, 74.0f));
		sprite->SetTextureRect((int)std::roundl(gridSize * 6), 0, (int)std::roundl(gridSize * 6), (int)std::roundl(gridSize * 7));
		break;
	case 4:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4.0f, gridSize * 3.0f));
		sprite->SetTextureRect((int)std::roundl(gridSize * 8), (int)std::roundl(gridSize * 7), (int)std::roundl(gridSize * 4), (int)std::roundl(gridSize * 3));
		break;
	case 5:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 7.0f, gridSize * 9.0f));
		sprite->SetTextureRect((int)std::roundl(gridSize * 12), 0, (int)std::roundl(gridSize * 7), (int)std::roundl(gridSize * 9));
		break;
	case 6:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4.0f, gridSize * 7.0f));
		sprite->SetTextureRect((int)std::roundl(gridSize * 19), 0, (int)std::roundl(gridSize * 4), (int)std::roundl(gridSize * 7));
		break;
	case 7:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 8.0f, gridSize * 4.0f));
		sprite->SetTextureRect(0, (int)std::roundl(gridSize * 7), (int)std::roundl(gridSize * 8), (int)std::roundl(gridSize * 4));
		break;
	case 8:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, -5.0f * scale.y), Vector2(gridSize * 3.0f + 14.0f, gridSize * 3.0f - 10.0f));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-67.5f * scale.x, 0.0f), Vector2(gridSize * 0.75f, gridSize * 3));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(67.5f * scale.x, 0.0f), Vector2(gridSize * 0.75f, gridSize * 3));
		sprite->SetTextureRect((int)std::roundl(gridSize * 12), (int)std::roundl(gridSize * 9), (int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 3));
		break;
	case 9:
		//building->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(gridSize * 2.0f, gridSize * 1.5f), false);             // Is now a tunnel
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-80.0f * scale.x, 0.0f), Vector2(gridSize * 3, gridSize * 3));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(80.0f * scale.x, 0.0f), Vector2(gridSize * 3, gridSize * 3));
		sprite->SetTextureRect((int)std::roundl(gridSize * 17), (int)std::roundl(gridSize * 9), (int)std::roundl(gridSize * 8), (int)std::roundl(gridSize * 3));
		break;
	case 10:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 3.0f, gridSize * 2.0f));
		sprite->SetTextureRect(0, (int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 3), (int)std::roundl(gridSize * 2));
		break;
	case 11:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(gridSize * 2.0f + 26.0f, gridSize * 2.0f + 26.0f));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-gridSize * scale.x * 1.2f, -gridSize * scale.y * 1.2f), Vector2(18.0f, 18.0f));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(-gridSize * scale.x * 1.2f, gridSize * scale.y * 1.2f), Vector2(18.0f, 18.0f));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(gridSize * scale.x * 1.2f, gridSize * scale.y * 1.2f), Vector2(18.0f, 18.0f));
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(gridSize * scale.x * 1.2f, -gridSize * scale.y * 1.2f), Vector2(18.0f, 18.0f));
		sprite->SetTextureRect((int)std::roundl(gridSize * 8), (int)std::roundl(gridSize * 10), (int)std::roundl(gridSize * 3), (int)std::roundl(gridSize * 3));
		break;
	default:
		break;
	}
	return building;
}

void TestScene::Start() {
	Scene::Start();
	Engine::GetInstance().GetInput().lock()->SetControllerActive(0, true);
	std::shared_ptr<GameObject> player = CreateCharacter("Player", 0, Vector2(700.0f, 0.0f)).lock();
	std::shared_ptr<CircleCollider> playerSensor = player->AddComponent<CircleCollider>().lock();
	playerSensor->Init(Vector2(), 200.0f, true);

	std::shared_ptr<GameObject> car = gameObjectManager->CreateGameObject("Car").lock();
	car->Init();
	std::shared_ptr<Transform2D> carTransform = car->GetComponent<Transform2D>().lock();
	carTransform->SetPosition(Vector2(600.0f, -300.0f));
	std::shared_ptr<RigidBody2D> carRB = car->AddComponent<RigidBody2D>().lock();
	carRB->Init(b2BodyType::b2_dynamicBody, 1.0f, 1.0f);
	car->AddComponent<PolygonCollider>().lock()->Init(Vector2(0.0f, 0.0f), {Vector2(-28.0f, -36.0f), Vector2(-8.0f, -58.0f), Vector2(34.0f, -60.0f), Vector2(64.0f, 0.0f), Vector2(34.0f, 60.0f), Vector2(-8.0f, 58.0f), Vector2(-28.0f, 36.0f), Vector2(-34.0f, 0.0f)}, false, 500.0f);
	car->AddComponent<PolygonCollider>().lock()->Init(Vector2(128.0f, 0.0f), {Vector2(-96.0f, -52.0f), Vector2(76.0f, -64.0f), Vector2(112.0f, -38.0f), Vector2(124, -24.0f), Vector2(124, 24.0f), Vector2(112.0f, 38.0f), Vector2(76.0f, 64.0f), Vector2(-96.0f, 52.0f)}, false, 100.0f);
	//carRB->SetMass(1500.0f);

	std::cout << std::to_string(carRB->GetMass());
	std::shared_ptr<SpriteRenderer> carSprite = car->AddComponent<SpriteRenderer>().lock();
	carSprite->Init("Images/Car.png", PivotPoint::Left, RenderLayer::MIDGROUND_LAYER);
	carSprite->SetPivotManually(34.0f, carSprite->GetHeight() / 2.0f);

	Vector2 wheelAnchor = Vector2(-90.0f, 0.0f);

	std::shared_ptr<GameObject> leftWheel = gameObjectManager->CreateGameObject("LeftWheel").lock();
	leftWheel->Init();
	std::shared_ptr<Transform2D> leftWheelTransform = leftWheel->GetComponent<Transform2D>().lock();
	leftWheelTransform->SetPosition(carTransform->GetPosition() + Vector2(184.0f, -56.0f));
	std::shared_ptr<RigidBody2D> leftWheelRB = leftWheel->AddComponent<RigidBody2D>().lock();
	leftWheelRB->Init(b2BodyType::b2_dynamicBody, 0.5f, 0.2f);
	std::shared_ptr<WheelJoint> leftWheelJoint = leftWheel->AddComponent<WheelJoint>().lock();
	leftWheelJoint->Init(carRB, wheelAnchor, Vector2::Right, false);
	leftWheel->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(32.0f, 10.0f), false, 5.0f);

	std::shared_ptr<GameObject> rightWheel = gameObjectManager->CreateGameObject("RightWheel").lock();
	rightWheel->Init();
	std::shared_ptr<Transform2D> rightWheelTransform = rightWheel->GetComponent<Transform2D>().lock();
	rightWheelTransform->SetPosition(carTransform->GetPosition() + Vector2(184.0f, 56.0f));
	std::shared_ptr<RigidBody2D> rightWheelRB = rightWheel->AddComponent<RigidBody2D>().lock();
	rightWheelRB->Init(b2BodyType::b2_dynamicBody, 0.5f, 0.2f);
	std::shared_ptr<WheelJoint> rightWheelJoint = rightWheel->AddComponent<WheelJoint>().lock();
	rightWheelJoint->Init(carRB, wheelAnchor, Vector2::Right, false);
	rightWheel->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(32.0f, 10.0f), false, 5.0f);

	std::shared_ptr<GameObject> backLeftWheel = gameObjectManager->CreateGameObject("BackLeftWheel").lock();
	backLeftWheel->Init();
	std::shared_ptr<Transform2D> backLeftWheelTransform = backLeftWheel->GetComponent<Transform2D>().lock();
	backLeftWheelTransform->SetPosition(carTransform->GetPosition() + Vector2(16.0f, -54.0f));
	std::shared_ptr<RigidBody2D> backLeftWheelRB = backLeftWheel->AddComponent<RigidBody2D>().lock();
	backLeftWheelRB->Init(b2BodyType::b2_dynamicBody, 0.5f, 0.2f);
	std::shared_ptr<WheelJoint> backLeftWheelJoint = backLeftWheel->AddComponent<WheelJoint>().lock();
	backLeftWheelJoint->Init(carRB, wheelAnchor, Vector2::Right);
	backLeftWheel->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(32.0f, 10.0f), false, 5.0f);

	std::shared_ptr<GameObject> backRightWheel = gameObjectManager->CreateGameObject("BackRightWheel").lock();
	backRightWheel->Init();
	std::shared_ptr<Transform2D> backRightWheelTransform = backRightWheel->GetComponent<Transform2D>().lock();
	backRightWheelTransform->SetPosition(carTransform->GetPosition() + Vector2(16.0f, 54.0f));
	std::shared_ptr<RigidBody2D> backRightWheelRB = backRightWheel->AddComponent<RigidBody2D>().lock();
	backRightWheelRB->Init(b2BodyType::b2_dynamicBody, 0.5f, 0.2f);
	std::shared_ptr<WheelJoint> backRightWheelJoint = backRightWheel->AddComponent<WheelJoint>().lock();
	backRightWheelJoint->Init(carRB, wheelAnchor, Vector2::Right);
	backRightWheel->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(32.0f, 10.0f), false, 5.0f);

	Vector2 scale = Vector2(6.0f, 6.0f);
	std::shared_ptr<GameObject> background = gameObjectManager->CreateGameObject("Background").lock();
	background->Init();
	std::shared_ptr<Transform2D> t2 = background->AddComponent<Transform2D>().lock();
	std::shared_ptr<SpriteRenderer> spriteRenderer = t2->AddComponent<SpriteRenderer>().lock();
	spriteRenderer->Init("Images/Street.png", PivotPoint::Centre, RenderLayer::BACKGROUND_LAYER, 0);
	spriteRenderer->GetSprite().setScale(sf::Vector2f(6.0f, 6.0f));
	const float ppm = Physics::PIXELS_PER_METRE, offset = ppm / 2.0f;

	CreateBuilding(3, Vector2(ppm * -2 + offset, ppm * 8)*1.5f, scale, 90.0f).lock()->GetComponent<SpriteRenderer>().lock()->SetColour(0.8f, 0.8f, 0.8f, 1.0f);
	CreateBuilding(2, Vector2(ppm * -2 + offset, offset)*1.5f, scale);
	Vector2 size = Vector2(0.0f, 0.0f);
	for(size_t i = 0; i < 2; i++) {
		for(size_t j = 0; j < 3; j++) {
			size = CreateBuilding(1, Vector2(ppm * -5 * 1.5f + offset * 2 + size.x * i, ppm * -11 * 1.5f + offset * 2 + size.y * j), scale).lock()->GetComponent<SpriteRenderer>().lock()->GetSize() + Vector2(ppm, ppm);
		}
	}
	CreateBuilding(3, Vector2(ppm * 4.0f, ppm * -8.0f + offset)*1.5f, scale);
	CreateBuilding(3, Vector2(ppm * 29, 0)*1.5f, scale, 90.0f);
	CreateBuilding(5, Vector2(ppm * 14, ppm * 7)*1.5f, scale, 90.0f);
	CreateBuilding(6, Vector2(ppm * 15 + offset, ppm * -7 - offset)*1.5f, scale);
	CreateBuilding(10, Vector2(ppm * 18 + offset, ppm * -5 - offset)*1.5f, scale, 90.0f);
	CreateBuilding(10, Vector2(ppm * 18 + offset, ppm * -9 - offset)*1.5f, scale, 90.0f);
	CreateBuilding(1, Vector2(ppm * 27, ppm * -10 - offset)*1.5f, scale);
	CreateBuilding(1, Vector2(ppm * 27, ppm * 10 + offset)*1.5f, scale);
	CreateBuilding(10, Vector2(ppm * 30 + offset, ppm * -10 - offset)*1.5f, scale);
	CreateBuilding(10, Vector2(ppm * 30 + offset, ppm * 10 + offset)*1.5f, scale);
	CreateBuilding(1, Vector2(ppm * -13, ppm * 10 + offset)*1.5f, scale);
	CreateBuilding(1, Vector2(ppm * -16, ppm * 10 + offset)*1.5f, scale);
	CreateBuilding(6, Vector2(ppm * -19, ppm * 2 - offset)*1.5f, scale, 90.0f);
	CreateBuilding(11, Vector2(ppm * -13, ppm * 2 - offset)*1.5f, scale);
	CreateBuilding(10, Vector2(ppm * -14 + offset, ppm * -7 - offset)*1.5f, scale);
	CreateBuilding(4, Vector2(ppm * -14 + offset, ppm * -10)*1.5f, scale);
	CreateBuilding(2, Vector2(ppm * -30 + offset, ppm * -9)*1.5f, scale);
	CreateBuilding(8, Vector2(ppm * -23 - offset, ppm * -9)*1.5f, scale, -90.0f);
	CreateBuilding(10, Vector2(ppm * -26 + offset, ppm * 10 + offset)*1.5f, scale);
	CreateBuilding(4, Vector2(ppm * -31 + offset, ppm * 10)*1.5f, scale);
	CreateBuilding(9, Vector2(ppm * -31 + offset, ppm * 4)*1.5f, scale, 90.0f);

	std::shared_ptr<GameObject> boundaries = gameObjectManager->CreateGameObject("Boundaries").lock();
	boundaries->Init();
	std::shared_ptr<RigidBody2D> rb = boundaries->AddComponent<RigidBody2D>().lock();
	rb->Init(b2BodyType::b2_kinematicBody);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(0, -736 * scale.y), Vector2(2880 * scale.x, ppm * 5), false);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(-1408 * scale.x, 8), Vector2(ppm * 5, 1504 * scale.y), false);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(0, 736 * scale.y), Vector2(2880 * scale.x, ppm * 5), false);
	boundaries->AddComponent<BoxCollider>().lock()->Init(Vector2(1408 * scale.x, 0), Vector2(ppm * 5, 1504 * scale.y), false);
	const float left = -38.5f * scale.x * 32.0f, top = - 17.5f * scale.y * 32.0f;
	const float right = -left, bottom = -top;
	for(size_t i = 0; i < 40; i++) {
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

Vector2 GetLateralVelocity(std::shared_ptr<RigidBody2D> r) {
	Vector2 currentRightNormal = r->GetRight();
	return currentRightNormal * currentRightNormal.Dot(r->GetVelocity());
}

void TestScene::FixedUpdate(const float & fixedDeltaTime) {
	Engine engine = Engine::GetInstance();
	std::shared_ptr<Input> input = engine.GetInput().lock();
	std::shared_ptr<GameObject> gameObject = driving ? gameObjectManager->GetGameObject("Car").lock() : gameObjectManager->GetGameObject("Player").lock();
	bool useController = input->GetControllerActive(0);

	// Movement Test

	std::shared_ptr<RigidBody2D> rb = gameObject->GetComponent<RigidBody2D>().lock();
	engine.GetGraphics().lock()->MoveCamera((driving ? ((rb->GetPosition() + (rb->GetForward() * rb->GetSpeed())) - engine.GetGraphics().lock()->GetCameraPosition()) : (gameObject->GetComponent<RigidBody2D>().lock()->GetPosition() - engine.GetGraphics().lock()->GetCameraPosition())) *  (fixedDeltaTime * 1.5f));
	const float maxVelocity = Physics::PIXELS_PER_METRE * 2.0f;
	const float speed = gameObject->GetComponent<RigidBody2D>().lock()->GetSpeed();
	float oldZoom = engine.GetGraphics().lock()->GetCameraZoom();
	float newZoom = (speed / maxVelocity) * 0.4f;
	engine.GetGraphics().lock()->SetCameraZoom(std::max<float>(1.0f, (oldZoom * (1.0f - fixedDeltaTime) + ((1.0f + newZoom) * fixedDeltaTime))));
	if(driving) {
		std::shared_ptr<GameObject> blw = gameObjectManager->GetGameObject("BackLeftWheel").lock();
		std::shared_ptr<GameObject> brw = gameObjectManager->GetGameObject("BackRightWheel").lock();
		std::shared_ptr<RigidBody2D> blwRB = blw->GetComponent<RigidBody2D>().lock();
		std::shared_ptr<RigidBody2D> brwRB = brw->GetComponent<RigidBody2D>().lock();
		std::shared_ptr<GameObject> flw = gameObjectManager->GetGameObject("LeftWheel").lock();
		std::shared_ptr<WheelJoint> flwJ = flw->GetComponent<WheelJoint>().lock();
		std::shared_ptr<GameObject> frw = gameObjectManager->GetGameObject("RightWheel").lock();
		std::shared_ptr<WheelJoint> frwJ = frw->GetComponent<WheelJoint>().lock();
		float lockAngle = 60.0f;
		float turnSpeedPerSec = 180.0f;
		float turnPerTimeStep = turnSpeedPerSec / fixedDeltaTime;
		float desiredAngle = 0;
		float massScale = rb->GetMass() * rb->GetSpeed() * Physics::METRES_PER_PIXEL;
		if(useController) {
			float forceScale = 7000.0f * input->GetAxis(0, ControllerButtons::ControllerAxis::RT);
			forceScale -=  2000.0f * input->GetAxis(0, ControllerButtons::ControllerAxis::LT);
			blwRB->AddForce(blwRB->GetForward() * forceScale, ForceType::IMPULSE_FORCE);
			brwRB->AddForce(brwRB->GetForward() * forceScale, ForceType::IMPULSE_FORCE);
			rb->AddForceAtPoint(rb->GetRight() * input->GetAxis(0, ControllerButtons::ControllerAxis::LeftStickH) * massScale, flw->GetComponent<Transform2D>().lock()->GetPosition(), ForceType::IMPULSE_FORCE);
			rb->AddForceAtPoint(rb->GetRight() * input->GetAxis(0, ControllerButtons::ControllerAxis::LeftStickH) * massScale, frw->GetComponent<Transform2D>().lock()->GetPosition(), ForceType::IMPULSE_FORCE);
		}
		else {
			if(input->GetKey(KeyCodes::KeyCode::W) || input->GetKey(KeyCodes::KeyCode::Up)) {
				blwRB->AddForce(blwRB->GetForward() * 7000.0f, ForceType::IMPULSE_FORCE);
				brwRB->AddForce(brwRB->GetForward() * 7000.0f, ForceType::IMPULSE_FORCE);
			}
			else if(input->GetKey(KeyCodes::KeyCode::S) || input->GetKey(KeyCodes::KeyCode::Down)) {
				blwRB->AddForce(-blwRB->GetForward() * 2000.0f, ForceType::IMPULSE_FORCE);
				brwRB->AddForce(-brwRB->GetForward() * 2000.0f, ForceType::IMPULSE_FORCE);
			}
			if(input->GetKey(KeyCodes::KeyCode::A) || input->GetKey(KeyCodes::KeyCode::Left)) {
				//carRB->AddTorque(-lockAngle * std::min(carRB->GetSpeed()* Physics::METRES_PER_PIXEL, 1.0f), ForceType::IMPULSE_FORCE);
				rb->AddForceAtPoint(-rb->GetRight() * massScale, flw->GetComponent<Transform2D>().lock()->GetPosition(), ForceType::IMPULSE_FORCE);
				rb->AddForceAtPoint(-rb->GetRight() * massScale, frw->GetComponent<Transform2D>().lock()->GetPosition(), ForceType::IMPULSE_FORCE);
			}
			else if(input->GetKey(KeyCodes::KeyCode::D) || input->GetKey(KeyCodes::KeyCode::Right)) {
				//carRB->AddTorque(lockAngle * std::min(carRB->GetSpeed() * Physics::METRES_PER_PIXEL, 1.0f), ForceType::IMPULSE_FORCE);
				rb->AddForceAtPoint(rb->GetRight() * massScale, flw->GetComponent<Transform2D>().lock()->GetPosition(), ForceType::IMPULSE_FORCE);
				rb->AddForceAtPoint(rb->GetRight() * massScale, frw->GetComponent<Transform2D>().lock()->GetPosition(), ForceType::IMPULSE_FORCE);
			}
		}	

		Vector2 lateralImpulse = GetLateralVelocity(rb) * rb->GetMass();
		rb->AddForce(-lateralImpulse * fixedDeltaTime, ForceType::IMPULSE_FORCE);
		rb->AddTorque(0.1f * rb->GetInertia() * -rb->GetAngularVelocity(), ForceType::IMPULSE_FORCE);
	}
	else {
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


	}
	
	Scene::FixedUpdate(fixedDeltaTime);
}

void TestScene::Update(const float & deltaTime) {
	Scene::Update(deltaTime);
	std::shared_ptr<Input> input = Engine::GetInstance().GetInput().lock();
	mousePosition = input->GetMousePosition();
	if(input->GetKeyDown(KeyCodes::KeyCode::C)) drawColliders = !drawColliders;
	if(input->GetKeyDown(KeyCodes::KeyCode::G)) input->SetControllerActive(0, !input->GetControllerActive(0));
	if(input->GetKeyDown(KeyCodes::KeyCode::I)) oldInputStyle = !oldInputStyle;
	std::shared_ptr<GameObject> player = gameObjectManager->GetGameObject("Player").lock();
	std::shared_ptr<GameObject> car = gameObjectManager->GetGameObject("Car").lock();
	if(!driving) {
		if((car->GetComponent<Transform2D>().lock()->GetPosition() - player->GetComponent<Transform2D>().lock()->GetPosition() + player->GetComponent<Transform2D>().lock()->GetForward() * Physics::PIXELS_PER_METRE).Magnitude() < Physics::PIXELS_PER_METRE * 2.0f) {
			if(input->GetKeyDown(KeyCodes::KeyCode::E) || input->GetControllerButtonDown(0, ControllerButtons::ControllerButton::Y)) {
				driving = true;
				player->SetEnabled(false);
			}
		}
	}
	else {
		if(input->GetKeyDown(KeyCodes::KeyCode::E) || input->GetControllerButtonDown(0, ControllerButtons::ControllerButton::Y)) {
			std::shared_ptr<Transform2D> carTransform = car->GetComponent<Transform2D>().lock();
			driving = false;
			player->GetComponent<RigidBody2D>().lock()->SetPosition(carTransform->GetPosition() + (-carTransform->GetRight() * Physics::PIXELS_PER_METRE) + (carTransform->GetForward() * Physics::PIXELS_PER_METRE));
			player->SetEnabled(true);
		}
	}
	//Test(deltaTime);
}

void TestScene::Render() {
	//Engine engine = Engine::GetInstance();
	//engine.GetGraphics().lock()->Draw("Total Time = " + std::to_string(engine.GetTimer().lock()->GetTotalTime()) + "	Delta Time = " + std::to_string(engine.GetTimer().lock()->GetDeltaTime()) + "	FPS = " + std::to_string(engine.GetFPS()), Vector2(100.0f, 650.0f), 30);
	Scene::Render();
}

void TestScene::Test(const float & deltaTime) {
	
	// Box2D test
	//if(input->GetMouseButtonDown(MouseButtons::MouseButton::Right)) physicsSystem->CreateBox(mousePosition.x, mousePosition.y, Physics::PIXELS_PER_METRE, Physics::PIXELS_PER_METRE);
}