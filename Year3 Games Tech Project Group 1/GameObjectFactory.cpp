#include "GameObjectFactory.h"
#include <SFML\Graphics\Texture.hpp>
std::weak_ptr<GameObject> GameObjectFactory::CreateCharacter(const std::string & name, const int & characterType, const bool & aiControlled, const Vector2 & position, const Vector2 & scale, const float & rotation) {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
	std::shared_ptr<GameObject> character = gameObjectManager.CreateGameObject(name).lock();
	character->Init(position, rotation, scale);
	
	std::shared_ptr<RigidBody2D> r = character->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_dynamicBody, false, 5.0f, 1.5f);
	std::shared_ptr<CircleCollider> c = character->AddComponent<CircleCollider>().lock();
	c->Init(Vector2(0.0f, 0.0f), 14.0f, false);
	std::shared_ptr<PolygonCollider> body = character->AddComponent<PolygonCollider>().lock();
	body->Init(Vector2(), {Vector2(-7.0f, -28.0f), Vector2(-14.0f, 0.0f), Vector2(-7.0f, 28.0f), Vector2(-5.0f, 29.0f), Vector2(1.0f, 28.0f), Vector2(8.0f, 0.0f), Vector2(1.0f, -28.0f), Vector2(-5.0f, -29.0f)});
	std::shared_ptr<SpriteRenderer> sr = character->AddComponent<SpriteRenderer>().lock();
	sr->Init("Images/Characters.png", PivotPoint::Centre, RenderLayer::FOREGROUND_LAYER, 85);
	sr->SetTextureRect(64 * characterType, 0, 64, 64);
	r->SetMass(10);
	std::shared_ptr<CharacterScript> cs = character->AddComponent<CharacterScript>().lock();
	cs->Start();
	cs->SetArtificiallyIntelligent(aiControlled);
	std::shared_ptr<HealthScript> hs = character->AddComponent<HealthScript>().lock();
	if(characterType == 0) {
		const int mask = (CollisionCategory::CATEGORY_ALL & ~CollisionCategory::CATEGORY_PLAYER);
		character->SetCollisionFilter(CollisionCategory::CATEGORY_PLAYER, mask);
		hs->Start();
		hs->SetHealth(100.0f);
		std::shared_ptr<PlayerScript> playerScript = character->AddComponent<PlayerScript>().lock();
		playerScript->Start();
	}
	else {
		const int mask = (CollisionCategory::CATEGORY_ALL & ~CollisionCategory::CATEGORY_AI_CHARACTER);
		character->SetCollisionFilter(CollisionCategory::CATEGORY_AI_CHARACTER, mask);
		hs->Start();
		hs->SetHealth(50.0f);
	}
	return character;
}

std::weak_ptr<GameObject> GameObjectFactory::CreateBuilding(const int & buildingNumber, const Vector2 & position, const Vector2 & scale, const float & rotation) {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
	if(buildingNumber < 1 || buildingNumber > 20) return std::weak_ptr<GameObject>();
	std::shared_ptr<GameObject> building = gameObjectManager.CreateGameObject("Building").lock();
	building->Init(position, rotation, scale);
	std::shared_ptr<SpriteRenderer> sprite = building->AddComponent<SpriteRenderer>().lock();
	sprite->Init("Images/Buildings.png", PivotPoint::Centre, RenderLayer::FOREGROUND_LAYER, 5000);
	std::shared_ptr<RigidBody2D> r = building->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_staticBody);
	building->SetCollisionCategory(CATEGORY_BUILDING);
	const float gridSize = 32.0f;
	switch(buildingNumber) {
	case 1:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(64.0f, 64.0f));
		sprite->SetTextureRect((int)std::roundl(gridSize * 3), (int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 2), (int)std::roundl(gridSize * 2));
		break;
	case 2:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 6.0f, gridSize * 5.0f));
		sprite->SetTextureRect(0, 0, (int)std::roundl(gridSize * 6), (int)std::roundl(gridSize * 5));
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
	case 12:
		building->AddComponent<PolygonCollider>().lock()->Init(Vector2(), {Vector2(gridSize * -2.5f, gridSize), Vector2(gridSize * 1.9f, gridSize), Vector2(gridSize * 1.9f, -gridSize), Vector2(gridSize * -2.5f, -gridSize)});
		sprite->SetTextureRect(0, (int)std::roundl(gridSize * 11), (int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 2));
		break;
	case 13:
		building->AddComponent<PolygonCollider>().lock()->Init(Vector2(), {Vector2(gridSize * -2.5f, gridSize), Vector2(gridSize * 1.9f, gridSize), Vector2(gridSize * 1.9f, -gridSize), Vector2(gridSize * -2.5f, -gridSize)});
		sprite->SetTextureRect(0, (int)std::roundl(gridSize * 13), (int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 2));
		break;
	case 14:
		building->AddComponent<PolygonCollider>().lock()->Init(Vector2(), {Vector2(gridSize * -2.5f, gridSize), Vector2(gridSize * 1.9f, gridSize), Vector2(gridSize * 1.9f, -gridSize), Vector2(gridSize * -2.5f, -gridSize)});
		sprite->SetTextureRect((int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 13), (int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 2));
		break;
	case 15:
		building->AddComponent<PolygonCollider>().lock()->Init(Vector2(), {Vector2(gridSize * -2.5f, gridSize), Vector2(gridSize * 1.9f, gridSize), Vector2(gridSize * 1.9f, -gridSize), Vector2(gridSize * -2.5f, -gridSize)});
		sprite->SetTextureRect((int)std::roundl(gridSize * 10), (int)std::roundl(gridSize * 13), (int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 2));
		break;
	case 16:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4.0f, gridSize * 11.0f));
		sprite->SetTextureRect((int)std::roundl(gridSize * 25), 0, (int)std::roundl(gridSize * 4), (int)std::roundl(gridSize * 11));
	case 17:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4.0f, gridSize * 11.0f));
		sprite->SetTextureRect((int)std::roundl(gridSize * 29), 0, (int)std::roundl(gridSize * 4), (int)std::roundl(gridSize * 11));
	case 18:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4.0f, gridSize * 11.0f));
		sprite->SetTextureRect((int)std::roundl(gridSize * 33), 0, (int)std::roundl(gridSize * 4), (int)std::roundl(gridSize * 11));
		break;
	case 19:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 6.0f, gridSize * 4.0f));
		sprite->SetTextureRect((int)std::roundl(gridSize * 25), 0, (int)std::roundl(gridSize * 6), (int)std::roundl(gridSize * 4));
		break;
	case 20:
		building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 6.0f, gridSize * 4.0f));
		sprite->SetTextureRect((int)std::roundl(gridSize * 31), 0, (int)std::roundl(gridSize * 6), (int)std::roundl(gridSize * 4));
		break;
	default:
		break;
	}
	return building;
}

std::weak_ptr<GameObject> GameObjectFactory::CreateBullet(const Vector2 & position, const Vector2 & scale, const float & rotation, const float & speed, const std::string & tag) {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
	std::shared_ptr<GameObject> bullet = gameObjectManager.CreateGameObject("Bullet").lock();
	bullet->Init(position, rotation, scale);
	bullet->SetTag(tag);
	std::shared_ptr<RigidBody2D> rb = bullet->AddComponent<RigidBody2D>().lock();
	rb->Init(b2_dynamicBody, true, 0.0f, 0.005f);
	rb->SetVelocity(bullet->GetComponent<Transform2D>().lock()->GetForward() * speed);
	std::shared_ptr<BoxCollider> bc = bullet->AddComponent<BoxCollider>().lock();
	bc->Init(Vector2(), Vector2(6.0f, 2.0f), false, 1000.0f);
	std::shared_ptr<SpriteRenderer> sprite = bullet->AddComponent<SpriteRenderer>().lock();
	sprite->Init("Images/Bullet.png", PivotPoint::Centre, RenderLayer::MIDGROUND_LAYER, 0);
	std::shared_ptr<AudioSource> as = bullet->AddComponent<AudioSource>().lock();
	as->Init("Audio/9mm.wav", true);
	as->SetVolume(0.5f);
	std::shared_ptr<BulletScript> bs = bullet->AddComponent<BulletScript>().lock();
	bs->Start();
	return bullet;
}

std::weak_ptr<GameObject> GameObjectFactory::CreateVehicle(const int & vehicleNumber, const Vector2 & position, const Vector2 & scale, const float & rotation) {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
	std::shared_ptr<GameObject> vehicle = gameObjectManager.CreateGameObject("Vehicle").lock();
	vehicle->Init(position + Vector2(0.0f, 68.0f), rotation, scale);
	std::shared_ptr<RigidBody2D> carRB = vehicle->AddComponent<RigidBody2D>().lock();
	carRB->Init(b2BodyType::b2_dynamicBody, true, 1.0f, 1.0f);
	vehicle->AddComponent<PolygonCollider>().lock()->Init(Vector2(-12.0f, 1.0f), {Vector2(-32.0f, -26.0f), Vector2(-14.0f, -52.0f), Vector2(34.0f, -58.0f), Vector2(64.0f, 50.0f), Vector2(64.0f, -50.0f), Vector2(34.0f, 58.0f), Vector2(-14.0f, 52.0f), Vector2(-32.0f, 26.0f)}, false, 3000.0f);
	vehicle->AddComponent<PolygonCollider>().lock()->Init(Vector2(116.0f, 0.0f), {Vector2(-100.0f, -48.0f), Vector2(76.0f, -64.0f), Vector2(112.0f, -38.0f), Vector2(124.0f, -24.0f), Vector2(124.0f, 24.0f), Vector2(112.0f, 38.0f), Vector2(76.0f, 64.0f), Vector2(-100.0f, 48.0f)}, false, 200.0f);

	std::shared_ptr<SpriteRenderer> carSprite = vehicle->AddComponent<SpriteRenderer>().lock();
	carSprite->Init("Images/Cars.png", PivotPoint::Left, RenderLayer::MIDGROUND_LAYER);
	carSprite->SetTextureRect(0, 132 * vehicleNumber, 288, 132);
	carSprite->SetPivotManually(44.0f, carSprite->GetHeight() / 2.0f);
	vehicle->AddComponent<VehicleController>().lock()->Start();
	vehicle->SetTag("Vehicle");
	std::shared_ptr<HealthScript> hs = vehicle->AddComponent<HealthScript>().lock();
	hs->SetHealth(25000.0f);
	return vehicle;
}
