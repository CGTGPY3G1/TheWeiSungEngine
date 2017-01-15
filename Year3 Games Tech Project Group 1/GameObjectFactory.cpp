#include "GameObjectFactory.h"

std::weak_ptr<GameObject> GameObjectFactory::CreateCharacter(const std::string & name, const int & characterType, const Vector2 & position, const Vector2 & scale, const float & rotation) {
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

std::weak_ptr<GameObject> GameObjectFactory::CreateBuilding(const int & buildingNumber, const Vector2 & position, const Vector2 & scale, const float & rotation) {
	GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
	if(buildingNumber < 1 || buildingNumber > 11) return std::weak_ptr<GameObject>();
	std::shared_ptr<GameObject> building = gameObjectManager.CreateGameObject("Building").lock();
	building->Init(position, rotation, scale);
	std::shared_ptr<SpriteRenderer> sprite = building->AddComponent<SpriteRenderer>().lock();
	sprite->Init("Images/Buildings.png", PivotPoint::Centre, RenderLayer::FOREGROUND_LAYER, 5000);
	std::shared_ptr<RigidBody2D> r = building->AddComponent<RigidBody2D>().lock();
	r->Init(b2BodyType::b2_kinematicBody);
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
	default:
		break;
	}
	return building;
}