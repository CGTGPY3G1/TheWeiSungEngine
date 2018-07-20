#include "GameObjectFactory.h"
#include <SFML\Graphics\Texture.hpp>
#include "WSMath.h"
namespace WeiSungEngine {
	std::vector<CollectableData> GameObjectFactory::data = std::vector<CollectableData>();

	std::weak_ptr<GameObject> GameObjectFactory::CreateCharacter(const std::string & name, const int & characterType, const bool & aiControlled, const Vector2 & position, const Vector2 & scale, const float & rotation) {
		if (characterType > 8) std::cout << "Invalid Character ID!" << std::endl;
		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		std::shared_ptr<GameObject> character = gameObjectManager.CreateGameObject(name).lock();
		character->Init(position, rotation, scale);

		std::shared_ptr<Transform2D> characterTransform = character->GetComponent<Transform2D>().lock();
		std::shared_ptr<RigidBody2D> r = character->AddComponent<RigidBody2D>().lock();
		r->Init(b2BodyType::b2_dynamicBody, false, 5.0f, 1.5f);
		std::shared_ptr<CircleCollider> c = character->AddComponent<CircleCollider>().lock();
		c->Init(Vector2(0.0f, 0.0f), 14.0f, false);
		std::shared_ptr<PolygonCollider> body = character->AddComponent<PolygonCollider>().lock();
		body->Init(Vector2(), { Vector2(-7.0f, -28.0f), Vector2(-14.0f, 0.0f), Vector2(-7.0f, 28.0f), Vector2(-5.0f, 29.0f), Vector2(1.0f, 28.0f), Vector2(8.0f, 0.0f), Vector2(1.0f, -28.0f), Vector2(-5.0f, -29.0f) });
		std::shared_ptr<SpriteRenderer> sr = character->AddComponent<SpriteRenderer>().lock();
		sr->Init("Images/Characters.png", PivotPoint::Centre, RenderLayer::FOREGROUND_LAYER, 85);
		const int tileSize = 64;
		sr->SetTextureRect(tileSize * characterType, 0, tileSize, tileSize);
		r->SetMass(10);



		std::shared_ptr<GameObject> hand = gameObjectManager.CreateGameObject("GunHand").lock();
		hand->Init(position + (characterTransform->GetForward() * 13.0f) + characterTransform->GetRight() * 21.0f, rotation, scale);
		hand->GetComponent<Transform2D>().lock()->SetParent(characterTransform);
		std::shared_ptr<WeaponCache> weaponCache = hand->AddComponent<WeaponCache>().lock();
		weaponCache->Start();

		std::shared_ptr<GameObject> sensor = gameObjectManager.CreateGameObject("CharacterSensor").lock();
		sensor->Init(position, rotation, scale);
		std::shared_ptr<RigidBody2D> sensorRB = sensor->AddComponent<RigidBody2D>().lock();
		sensorRB->Init();
		std::shared_ptr<CircleCollider> cc = sensor->AddComponent<CircleCollider>().lock();
		cc->Init(Vector2(0.0f, 0.0f), 8.0f * Physics::PIXELS_PER_METRE, true);
		sensor->GetComponent<Transform2D>().lock()->SetParent(characterTransform);

		const float animSpeed = 0.3f;
		Animation idle = Animation("Idle");
		idle.looping = false;
		idle.AddFrame(AnimationFrame(animSpeed, tileSize * characterType, 0, tileSize, tileSize));
		Animation idleWithGun = Animation("IdleWithGun");
		idleWithGun.looping = false;
		idleWithGun.AddFrame(AnimationFrame(0.4f, tileSize * characterType, tileSize * 3, tileSize, tileSize));
		Animation walk = Animation("Walk");
		walk.AddFrame(AnimationFrame(animSpeed, tileSize * characterType, tileSize, tileSize, tileSize));
		walk.AddFrame(AnimationFrame(animSpeed, tileSize * characterType, 0, tileSize, tileSize));
		walk.AddFrame(AnimationFrame(animSpeed, tileSize * characterType, tileSize * 2, tileSize, tileSize));
		walk.AddFrame(AnimationFrame(animSpeed, tileSize * characterType, 0, tileSize, tileSize));
		walk.looping = true;
		Animation walkWithGun = Animation("WalkWithGun");
		walkWithGun.AddFrame(AnimationFrame(animSpeed, tileSize * characterType, tileSize * 4, tileSize, tileSize));
		walkWithGun.AddFrame(AnimationFrame(animSpeed, tileSize * characterType, tileSize * 3, tileSize, tileSize));
		walkWithGun.AddFrame(AnimationFrame(animSpeed, tileSize * characterType, tileSize * 5, tileSize, tileSize));
		walkWithGun.AddFrame(AnimationFrame(animSpeed, tileSize * characterType, tileSize * 3, tileSize, tileSize));
		walkWithGun.looping = true;
		std::shared_ptr<SpriteAnimator> sa = character->AddComponent<SpriteAnimator>().lock();
		sa->AddAnimation(idle);
		sa->AddAnimation(idleWithGun);
		sa->AddAnimation(walk);
		sa->AddAnimation(walkWithGun);
		sa->PlayAnimation("Idle");
		sa->Start();

		std::shared_ptr<CharacterScript> cs = character->AddComponent<CharacterScript>().lock();
		cs->SetArtificiallyIntelligent(aiControlled);
		cs->Start();
		cs->SetGunHandTransform(hand->GetComponent<Transform2D>().lock());
		std::shared_ptr<BloodSplatterScript> bs = character->AddComponent<BloodSplatterScript>().lock();
		bs->Start();
		std::shared_ptr<HealthScript> hs = character->AddComponent<HealthScript>().lock();
		if (characterType == 0) {
			const int mask = (CollisionCategory::CATEGORY_ALL & ~CollisionCategory::CATEGORY_PLAYER);
			character->SetCollisionFilter(CollisionCategory::CATEGORY_PLAYER, mask);
			hs->Start();
			hs->SetHealth(1000.0);
			std::shared_ptr<PlayerScript> playerScript = character->AddComponent<PlayerScript>().lock();
			playerScript->Start();
			weaponCache->AddWeapon(WeaponType::WeaponTypePistol, 10000);
			weaponCache->AddWeapon(WeaponType::WeaponTypeUzi, 10000);
			weaponCache->AddWeapon(WeaponType::WeaponTypeGrenade, 100);
		}
		else {
			const int mask = (CollisionCategory::CATEGORY_ALL & ~CollisionCategory::CATEGORY_AI_CHARACTER);
			character->SetCollisionFilter(CollisionCategory::CATEGORY_AI_CHARACTER, mask);
			hs->Start();
			hs->SetHealth(150.0f);
			const int r = Random::RandomInt(100);
			if (r < 70) weaponCache->AddWeapon(WeaponType::WeaponTypePistol, 1000);
			else if (r < 90) weaponCache->AddWeapon(WeaponType::WeaponTypeUzi, 1000);
			else weaponCache->AddWeapon(WeaponType::WeaponTypeGrenade, 50);
		}
		character->AddComponent<DamageScript>().lock()->Start();
		return character;
	}

	std::weak_ptr<GameObject> GameObjectFactory::CreateBuilding(const int & buildingNumber, const Vector2 & position, const Vector2 & scale, const float & rotation) {
		static int sortOffset = 0;
		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		if (buildingNumber < 1 || buildingNumber > 20) return std::weak_ptr<GameObject>();
		std::shared_ptr<GameObject> building = gameObjectManager.CreateGameObject("Building").lock();
		building->Init(position, rotation, scale);
		std::shared_ptr<SpriteRenderer> sprite = building->AddComponent<SpriteRenderer>().lock();
		sprite->Init("Images/Buildings.png", PivotPoint::Centre, RenderLayer::FOREGROUND_LAYER, 100000 + sortOffset++, true, false);
		std::shared_ptr<RigidBody2D> r = building->AddComponent<RigidBody2D>().lock();
		r->Init(b2BodyType::b2_kinematicBody);
		building->SetCollisionCategory(CATEGORY_BUILDING);
		const float gridSize = 32.0f;
		switch (buildingNumber) {
		case 1:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(64.0f, 64.0f));
			sprite->SetTextureRect((int)std::roundl(gridSize * 4), (int)std::roundl(gridSize * 6), (int)std::roundl(gridSize * 2), (int)std::roundl(gridSize * 2));
			break;
		case 2:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 6 - 0.5f, gridSize * 5 - 0.5f));
			sprite->SetTextureRect(0, 0, (int)std::roundl(gridSize * 6), (int)std::roundl(gridSize * 5));
			break;
		case 3:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(-60.0f * scale.x, 0.0f), Vector2(72.0f, 124.0f));
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(12.0f * scale.x, -75.0f * scale.y), Vector2(168.0f, 74.0f));
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(12.0f * scale.x, 75.0f * scale.y), Vector2(168.0f, 74.0f));
			sprite->SetTextureRect((int)std::roundl(gridSize * 7), 0, (int)std::roundl(gridSize * 6), (int)std::roundl(gridSize * 7));
			break;
		case 4:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4.0f, gridSize * 3.0f));
			sprite->SetTextureRect((int)std::roundl(gridSize * 9), (int)std::roundl(gridSize * 8), (int)std::roundl(gridSize * 4), (int)std::roundl(gridSize * 3));
			break;
		case 5:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 7.0f, gridSize * 9.0f));
			sprite->SetTextureRect((int)std::roundl(gridSize * 14), 0, (int)std::roundl(gridSize * 7), (int)std::roundl(gridSize * 9));
			break;
		case 6:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4.0f, gridSize * 7.0f));
			sprite->SetTextureRect((int)std::roundl(gridSize * 22), 0, (int)std::roundl(gridSize * 4), (int)std::roundl(gridSize * 7));
			break;
		case 7:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 8.0f, gridSize * 4.0f));
			sprite->SetTextureRect(0, (int)std::roundl(gridSize * 9), (int)std::roundl(gridSize * 8), (int)std::roundl(gridSize * 4));
			break;
		case 8:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, -5.0f * scale.y), Vector2(gridSize * 3.0f + 14.0f, gridSize * 3.0f - 10.0f));
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(-67.5f * scale.x, 0.0f), Vector2(gridSize * 0.76f, gridSize * 3));
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(67.5f * scale.x, 0.0f), Vector2(gridSize * 0.76f, gridSize * 3));
			sprite->SetTextureRect((int)std::roundl(gridSize * 14), (int)std::roundl(gridSize * 10), (int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 3));
			break;
		case 9:
			//building->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(gridSize * 2.0f, gridSize * 1.5f), false);             // Is now a tunnel
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(-80.0f * scale.x, 0.0f), Vector2(gridSize * 3, gridSize * 3));
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(80.0f * scale.x, 0.0f), Vector2(gridSize * 3, gridSize * 3));
			sprite->SetTextureRect((int)std::roundl(gridSize * 20), (int)std::roundl(gridSize * 12), (int)std::roundl(gridSize * 8), (int)std::roundl(gridSize * 3));
			break;
		case 10:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 3.0f, gridSize * 2.0f));
			sprite->SetTextureRect(0, (int)std::roundl(gridSize * 6), (int)std::roundl(gridSize * 3), (int)std::roundl(gridSize * 2));
			break;
		case 11:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(0.0f, 0.0f), Vector2(gridSize * 2.0f + 26.5f, gridSize * 2.0f + 26.5f));
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(-gridSize * scale.x * 1.2f, -gridSize * scale.y * 1.2f), Vector2(18.5f, 18.5f));
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(-gridSize * scale.x * 1.2f, gridSize * scale.y * 1.2f), Vector2(18.5f, 18.5f));
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(gridSize * scale.x * 1.2f, gridSize * scale.y * 1.2f), Vector2(18.5f, 18.5f));
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(gridSize * scale.x * 1.2f, -gridSize * scale.y * 1.2f), Vector2(18.5f, 18.5f));
			sprite->SetTextureRect((int)std::roundl(gridSize * 23), (int)std::roundl(gridSize * 8), (int)std::roundl(gridSize * 3), (int)std::roundl(gridSize * 3));
			break;
		case 12:
			building->AddComponent<PolygonCollider>().lock()->Init(Vector2(), { Vector2(gridSize * -2.5f, gridSize), Vector2(gridSize * 1.9f, gridSize), Vector2(gridSize * 1.9f, -gridSize), Vector2(gridSize * -2.5f, -gridSize) });
			sprite->SetTextureRect(0, (int)std::roundl(gridSize * 14), (int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 2));
			break;
		case 13:
			building->AddComponent<PolygonCollider>().lock()->Init(Vector2(), { Vector2(gridSize * -2.5f, gridSize), Vector2(gridSize * 1.9f, gridSize), Vector2(gridSize * 1.9f, -gridSize), Vector2(gridSize * -2.5f, -gridSize) });
			sprite->SetTextureRect(0, (int)std::roundl(gridSize * 17), (int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 2));
			break;
		case 14:
			building->AddComponent<PolygonCollider>().lock()->Init(Vector2(), { Vector2(gridSize * -2.5f, gridSize), Vector2(gridSize * 1.9f, gridSize), Vector2(gridSize * 1.9f, -gridSize), Vector2(gridSize * -2.5f, -gridSize) });
			sprite->SetTextureRect((int)std::roundl(gridSize * 6), (int)std::roundl(gridSize * 17), (int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 2));
			break;
		case 15:
			building->AddComponent<PolygonCollider>().lock()->Init(Vector2(), { Vector2(gridSize * -2.5f, gridSize), Vector2(gridSize * 1.9f, gridSize), Vector2(gridSize * 1.9f, -gridSize), Vector2(gridSize * -2.5f, -gridSize) });
			sprite->SetTextureRect((int)std::roundl(gridSize * 12), (int)std::roundl(gridSize * 17), (int)std::roundl(gridSize * 5), (int)std::roundl(gridSize * 2));
			break;
		case 16:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4.0f, gridSize * 11.0f));
			sprite->SetTextureRect((int)std::roundl(gridSize * 27), 0, (int)std::roundl(gridSize * 4), (int)std::roundl(gridSize * 11));
			break;
		case 17:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4.0f, gridSize * 11.0f));
			sprite->SetTextureRect((int)std::roundl(gridSize * 32), 0, (int)std::roundl(gridSize * 4), (int)std::roundl(gridSize * 11));
			break;
		case 18:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 4.0f, gridSize * 11.0f));
			sprite->SetTextureRect((int)std::roundl(gridSize * 37), 0, (int)std::roundl(gridSize * 4), (int)std::roundl(gridSize * 11));
			break;
		case 19:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 6.0f, gridSize * 4.0f));
			sprite->SetTextureRect((int)std::roundl(gridSize * 29), (int)std::roundl(gridSize * 12), (int)std::roundl(gridSize * 6), (int)std::roundl(gridSize * 4));
			break;
		case 20:
			building->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(gridSize * 6.0f, gridSize * 4.0f));
			sprite->SetTextureRect((int)std::roundl(gridSize * 36), (int)std::roundl(gridSize * 12), (int)std::roundl(gridSize * 6), (int)std::roundl(gridSize * 4));
			break;
		default:
			break;
		}
		building->AddComponent<DamageScript>().lock()->Start();
		return building;
	}

	std::weak_ptr<GameObject> GameObjectFactory::CreateBarrier(const int & barrierType, const Vector2 & position, const Vector2 & scale, const float & rotation) {
		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		if (barrierType < 1 || barrierType > 6) return std::weak_ptr<GameObject>();
		std::shared_ptr<GameObject> barrier = gameObjectManager.CreateGameObject("Barrier").lock();
		barrier->Init(position, rotation, scale);
		std::shared_ptr<SpriteRenderer> sprite = barrier->AddComponent<SpriteRenderer>().lock();
		sprite->Init("Images/Buildings.png", PivotPoint::Centre, RenderLayer::FOREGROUND_LAYER, 100000);
		sprite->SetColour(0.6f, 0.0f, 0.0f, 1.0f);
		const int gridSize = 32;
		switch (barrierType) {
		case 1:
		{
			std::shared_ptr<RigidBody2D> r = barrier->AddComponent<RigidBody2D>().lock();
			r->Init(b2BodyType::b2_staticBody);
			barrier->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(32.0f, 32.0f));
			sprite->SetTextureRect(gridSize * 9, gridSize * 12, gridSize, gridSize);
			sprite->SetColour(0.75f, 0.0f, 0.0f, 1.0f);
		}
		break;
		case 2:
		{
			std::shared_ptr<RigidBody2D> r = barrier->AddComponent<RigidBody2D>().lock();
			r->Init(b2BodyType::b2_staticBody);
			barrier->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(32.0f, 32.0f));
			sprite->SetTextureRect(gridSize * 11, gridSize * 12, gridSize, gridSize);
			sprite->SetColour(0.75f, 0.0f, 0.0f, 1.0f);
		}
		break;
		case 3:
			sprite->SetTextureRect(gridSize * 9, gridSize * 14, gridSize, gridSize);
			break;
		case 4:
			sprite->SetTextureRect(gridSize * 11, gridSize * 14, gridSize, gridSize);
			break;
		case 5:
		{
			std::shared_ptr<RigidBody2D> r = barrier->AddComponent<RigidBody2D>().lock();
			r->Init(b2BodyType::b2_staticBody);
			barrier->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(24.0f, 32.0f));
			sprite->SetTextureRect(gridSize * 13, gridSize * 14, gridSize, gridSize);
		}
		break;
		case 6:
		{
			std::shared_ptr<RigidBody2D> r = barrier->AddComponent<RigidBody2D>().lock();
			r->Init(b2BodyType::b2_staticBody);
			barrier->AddComponent<BoxCollider>().lock()->Init(Vector2(), Vector2(32.0f, 24.0f));
			sprite->SetTextureRect(gridSize * 15, gridSize * 14, gridSize, gridSize);
		}
		break;
		default:
			break;
		}
		barrier->SetCollisionCategory(CATEGORY_BUILDING);
		barrier->AddComponent<DamageScript>().lock()->Start();
		return barrier;
	}

	std::weak_ptr<GameObject> GameObjectFactory::CreateBullet(const std::weak_ptr<GameObject> & creator, const std::string & creatorName, const Vector2 & position, const Vector2 & scale, const float & rotation, const float & speed, const std::string & tag) {
		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		std::shared_ptr<GameObject> bullet = gameObjectManager.CreateGameObject("Bullet").lock();
		bullet->Init(position, rotation, scale);
		bullet->SetTag(tag);
		std::shared_ptr<RigidBody2D> rb = bullet->AddComponent<RigidBody2D>().lock();
		rb->Init(b2_dynamicBody, true, 0.0f, 0.005f);
		rb->SetVelocity(bullet->GetComponent<Transform2D>().lock()->GetForward() * speed);
		std::shared_ptr<BoxCollider> bc = bullet->AddComponent<BoxCollider>().lock();
		bc->Init(Vector2(), Vector2(12.0f, 6.0f), false, 400.0f);
		std::shared_ptr<SpriteRenderer> sprite = bullet->AddComponent<SpriteRenderer>().lock();
		sprite->Init("Images/Bullet.png", PivotPoint::Centre, RenderLayer::MIDGROUND_LAYER, 0);
		std::shared_ptr<AudioSource> as = bullet->AddComponent<AudioSource>().lock();
		as->Init("Audio/9mm.ogg", true);
		as->SetVolume(0.5f);
		as->SetAttenuation(7.5f * Physics::PIXELS_PER_METRE, 10.0f);
		std::shared_ptr<BulletScript> bs = bullet->AddComponent<BulletScript>().lock();
		bs->Start();
		bullet->AddComponent<DamageScript>().lock()->Start();
		if (creator.use_count() > 0) {
			std::shared_ptr<AttackerIdentityScript> ais = bullet->AddComponent<AttackerIdentityScript>().lock();
			ais->SetAttacker(creator, creatorName, AttackType::ProjectileHit);
			ais->Start();
		}
		return bullet;
	}

	std::weak_ptr<GameObject> GameObjectFactory::CreateGrenade(const std::weak_ptr<GameObject>& creator, const std::string & creatorName, const Vector2 & position, const Vector2 & scale, const float & rotation, const float & speed, const std::string & tag) {
		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		std::shared_ptr<GameObject> grenade = gameObjectManager.CreateGameObject("Grenade").lock();
		grenade->Init(position, rotation, scale);
		grenade->SetTag(tag);
		std::shared_ptr<RigidBody2D> rb = grenade->AddComponent<RigidBody2D>().lock();
		rb->Init(b2_dynamicBody, true, 0.3f, 0.6f);
		rb->SetVelocity(grenade->GetComponent<Transform2D>().lock()->GetForward() * speed);
		std::shared_ptr<CircleCollider> cc = grenade->AddComponent<CircleCollider>().lock();
		cc->Init(Vector2(), 10, false, 100.0f, 0.3f, 0.5f);
		rb->SetAngularVelocity(WSMath::TAU());
		grenade->AddComponent<GrenadeScript>().lock()->Start();
		std::shared_ptr<SpriteRenderer> sprite = grenade->AddComponent<SpriteRenderer>().lock();
		sprite->Init("Images/Weapons.png", PivotPoint::Centre, RenderLayer::MIDGROUND_LAYER, 0);
		sprite->SetTextureRect(100, 60, 14, 8);
		if (creator.use_count() > 0) {
			std::shared_ptr<AttackerIdentityScript> ais = grenade->AddComponent<AttackerIdentityScript>().lock();
			ais->SetAttacker(creator, creatorName, AttackType::GrenadeHit);
			ais->Start();
		}
		return grenade;
	}

	std::weak_ptr<GameObject> GameObjectFactory::CreateVehicle(const int & vehicleNumber, const Vector2 & position, const Vector2 & scale, const float & rotation) {
		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		std::shared_ptr<GameObject> vehicle = gameObjectManager.CreateGameObject("Vehicle").lock();
		vehicle->Init(position + Vector2(-96.0f, 0.0f).RotatedInDegrees(rotation), rotation, scale);
		std::shared_ptr<RigidBody2D> carRB = vehicle->AddComponent<RigidBody2D>().lock();
		carRB->Init(b2BodyType::b2_dynamicBody, true, 1.0f, 1.0f);
		vehicle->AddComponent<PolygonCollider>().lock()->Init(Vector2(-12.0f, 1.0f), { Vector2(-32.0f, -26.0f), Vector2(-14.0f, -52.0f), Vector2(34.0f, -58.0f), Vector2(64.0f, 50.0f), Vector2(64.0f, -50.0f), Vector2(34.0f, 58.0f), Vector2(-14.0f, 52.0f), Vector2(-32.0f, 26.0f) }, false, 3000.0f);
		vehicle->AddComponent<PolygonCollider>().lock()->Init(Vector2(116.0f, 0.0f), { Vector2(-100.0f, -48.0f), Vector2(76.0f, -64.0f), Vector2(112.0f, -38.0f), Vector2(124.0f, -24.0f), Vector2(124.0f, 24.0f), Vector2(112.0f, 38.0f), Vector2(76.0f, 64.0f), Vector2(-100.0f, 48.0f) }, false, 200.0f);

		std::shared_ptr<SpriteRenderer> carSprite = vehicle->AddComponent<SpriteRenderer>().lock();
		carSprite->Init("Images/Cars.png", PivotPoint::TopLeft, RenderLayer::MIDGROUND_LAYER);
		carSprite->SetTextureRect(0, 132 * vehicleNumber, 288, 132);
		carSprite->SetPivotManually(44.0f, carSprite->GetHeight() / 2.0f);
		vehicle->AddComponent<VehicleController>().lock()->Start();
		vehicle->SetTag("Vehicle");
		std::shared_ptr<HealthScript> hs = vehicle->AddComponent<HealthScript>().lock();
		hs->SetHealth(10000.0f);
		vehicle->AddComponent<DamageScript>().lock()->Start();
		std::shared_ptr<AttackerIdentityScript> ais = vehicle->AddComponent<AttackerIdentityScript>().lock();
		ais->SetAttacker();
		ais->Start();
		return vehicle;
	}

	std::weak_ptr<GameObject> GameObjectFactory::CreateBloodSplat(const Vector2 & position, const int & type, const int & version) {
		static int sortOrder = -1000000;
		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		std::shared_ptr<GameObject> splat = gameObjectManager.CreateGameObject("BloodSplat").lock();
		const float scale = Random::RandomFloat(1.0f, 1.5f);
		splat->Init(position, Random::RandomFloat(360.0f), Vector2(scale, scale));
		std::shared_ptr<SpriteRenderer> sr = splat->AddComponent<SpriteRenderer>().lock();
		sr->Init("Images/Blood.png", PivotPoint::Centre, RenderLayer::BACKGROUND_LAYER, sortOrder++);
		if (type == 0) {
			sr->SetTextureRect(0, 0, 64, 64);
		}
		else {
			int x = (version < 0 || version > 5) ? Random::RandomInt(6) : version;
			sr->SetTextureRect(x * 32, 64, 32, 32);
		}

		std::shared_ptr<DeathTimer> dt = splat->AddComponent<DeathTimer>().lock();
		dt->SetTime(45.0f);
		return splat;
	}

	std::weak_ptr<GameObject> GameObjectFactory::CreateBloodSpray(const Vector2 & position, const Vector2 & direction, const Vector2 & scale) {
		static int sortOrder = -1000000;
		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		std::shared_ptr<GameObject> spray = gameObjectManager.CreateGameObject("BloodSplat").lock();
		spray->Init(position, direction.AngleInDegrees(), scale);
		std::shared_ptr<SpriteRenderer> sr = spray->AddComponent<SpriteRenderer>().lock();
		sr->Init("Images/Blood.png", PivotPoint::TopLeft, RenderLayer::BACKGROUND_LAYER, sortOrder++);
		sr->SetTextureRect(0, 96, 128, 64);
		sr->SetPivotManually(Vector2(16.0f, 32.0f));
		std::shared_ptr<DeathTimer> dt = spray->AddComponent<DeathTimer>().lock();
		dt->SetTime(45.0f);
		return spray;
	}

	std::weak_ptr<GameObject> GameObjectFactory::CreateExplosionAnim(const Vector2 & position, const float & rotation) {
		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		std::shared_ptr<GameObject> explosion = gameObjectManager.CreateGameObject("Explosion").lock();
		explosion->Init(position, rotation, Vector2(8.0f, 8.0f));

		std::shared_ptr<Transform2D> characterTransform = explosion->GetComponent<Transform2D>().lock();
		std::shared_ptr<SpriteRenderer> sr = explosion->AddComponent<SpriteRenderer>().lock();
		sr->Init("Images/ExplosionAnim.png", PivotPoint::Centre, RenderLayer::FOREGROUND_LAYER, 1000);
		const int tileSize = 64;
		const float animSpeed = 0.1f;
		sr->SetTextureRect(0, 0, tileSize, tileSize);
		Animation eplosionAnim = Animation("Eplosion");
		for (size_t i = 0; i < 10; i++) {
			eplosionAnim.AddFrame(AnimationFrame(animSpeed, i * tileSize, 0, tileSize, tileSize));
		}
		eplosionAnim.looping = false;
		std::shared_ptr<SpriteAnimator> sa = explosion->AddComponent<SpriteAnimator>().lock();
		sa->AddAnimation(eplosionAnim);
		sa->PlayAnimation("Eplosion");
		sa->Start();
		std::shared_ptr<AudioSource> as = explosion->AddComponent<AudioSource>().lock();
		as->Init("Audio/Explosion.ogg", true);
		as->SetAttenuation(8.0f * Physics::PIXELS_PER_METRE, 2.0f);
		explosion->AddComponent<SelfDestructingAnimScript>().lock()->Start();
		return explosion;
	}

	void GameObjectFactory::ScheduleCollectableCreation(const std::string & name, const CollectableType & type, const unsigned int amountOfType, const Vector2 & position, const Vector2 & scale) {
		CollectableData newData;
		newData.name = name;
		newData.type = type;
		newData.amountOfType = amountOfType;
		newData.position = position;
		newData.scale = scale;
		data.push_back(newData);
	}

	void GameObjectFactory::CreateScheduledCollectables() {
		for (size_t i = 0; i < data.size(); i++) {
			CollectableData toCreate = data[i];
			CreateCollectable(toCreate.name, toCreate.type, toCreate.amountOfType, toCreate.position, toCreate.scale);
		}
		data.clear();
	}

	std::weak_ptr<GameObject> GameObjectFactory::CreateCollectable(const std::string & name, const CollectableType & type, const unsigned int amountOfType, const Vector2 & position, const Vector2 & scale) {
		GameObjectManager & gameObjectManager = GameObjectManager::GetInstance();
		std::shared_ptr<GameObject> collectable = gameObjectManager.CreateGameObject("Collectable").lock();
		collectable->Init(position, 0.0f, scale);
		collectable->SetTag("Collectable");
		std::shared_ptr<Collectable> c = collectable->AddComponent<Collectable>().lock();
		c->Init(type, amountOfType);
		std::shared_ptr<SpriteRenderer> sprite = collectable->AddComponent<SpriteRenderer>().lock();
		sprite->Init("Images/Items.png", PivotPoint::Centre, RenderLayer::MIDGROUND_LAYER, -5, true, false);
		sprite->SetTextureRect(128, 0, 32, 32);
		sprite->SetColour(1.0f, 1.0f, 1.0f, 0.5f);
		std::shared_ptr<SpriteRenderer> sprite2 = collectable->AddComponent<SpriteRenderer>().lock();
		sprite2->Init("Images/Items.png", PivotPoint::Centre, RenderLayer::MIDGROUND_LAYER, -4, true, false);
		sprite2->SetTextureRect(32 * (int)type, 0, 32, 32);
		sprite2->SetColour(1.0f, 1.0f, 1.0f, 0.5f);
		std::shared_ptr<RigidBody2D> rb = collectable->AddComponent<RigidBody2D>().lock();
		rb->Init(b2BodyType::b2_kinematicBody);
		std::shared_ptr<BoxCollider> box = rb->AddComponent<BoxCollider>().lock();
		box->Init(Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), true);
		return collectable;
	}
}
