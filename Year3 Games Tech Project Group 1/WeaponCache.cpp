#include "WeaponCache.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "SpriteRenderer.h"
#include "Transform2D.h"
#include <algorithm>
#include "Weapons.h"

WeaponCache::WeaponCache() {
}

WeaponCache::WeaponCache(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
	std::shared_ptr<SpriteRenderer> sr = AddComponent<SpriteRenderer>().lock();
	sr->Init("Images/Weapons.png", PivotPoint::Centre, RenderLayer::FOREGROUND_LAYER, 40000);
	transform = GetComponent<Transform2D>();
}

WeaponCache::~WeaponCache() {
}

void WeaponCache::Start() {
	AddWeapon(WeaponType::WeaponTypeNull, 0);
	UpdateSprite();
}

void WeaponCache::Update(const float & deltaTime) {
	if(currentWeapon.use_count() != 0) {
		std::shared_ptr<Weapon> w = currentWeapon.lock();
		if(w) w->UpdateTimers(deltaTime);
	}
}

int WeaponCache::GetSortOrder() {
	return TypeInfo::ScriptSortOrder<WeaponCache>();
}

void WeaponCache::Fire() {
	if(currentWeapon.use_count() > 0) {
		std::shared_ptr<Weapon> w = currentWeapon.lock();
		if(w->CanFire()) {			
			const WeaponType type = w->GetType();
			switch(type) {
			case WeaponType::WeaponTypePistol:
			case WeaponType::WeaponTypeUzi:
				GameObjectFactory::CreateBullet(transform.lock()->GetPosition() + transform.lock()->GetForward() * w->DistanceToFirePoint(), Vector2::One, transform.lock()->GetForward().AngleInDegrees() + Random::RandomFloat(-4.0f, 4.0f), 40.0f * Physics::PIXELS_PER_METRE, gameObject.lock()->GetTag()+"Bullet");
				w->Fire();
				break;
			default:
				break;
			}
		}
		else if(w->NeedsReloading()) w->Reload();
	}
}

void WeaponCache::AddAmmo(const WeaponType & weaponType, const int & amount) {
	if(amount == 0) return;
	std::vector<std::shared_ptr<Weapon>>::iterator it = std::find_if(weapons.begin(), weapons.end(), WeaponIsOfType(weaponType));
	if(it != weapons.end()) (*it)->AddAmmo(amount);	
}

std::weak_ptr<Weapon> WeaponCache::GetCurrentWeapon() {
	return currentWeapon;
}

const unsigned int WeaponCache::NumberOfWeapons() const {
	return weapons.size();
}

void WeaponCache::SwitchWeapon(const bool & forward) {
	const unsigned int noOfWeapons = NumberOfWeapons();
	if(noOfWeapons > 0) {	
		if(forward) {
			selectedWeapon++;
			if(selectedWeapon >= noOfWeapons) selectedWeapon = 0;
		}
		else {
			selectedWeapon--;
			if(selectedWeapon < 0) selectedWeapon = noOfWeapons - 1;
		}
		currentWeapon = weapons[selectedWeapon];
	}
	UpdateSprite();
}

void WeaponCache::AddWeapon(const WeaponType & weaponType, const int & ammo) {
		std::shared_ptr<Weapon> weapon;
		if(NumberOfWeapons() == 0) {
			switch(weaponType) {
			case WeaponType::WeaponTypePistol:
				weapon = std::static_pointer_cast<Weapon>(std::make_shared<Pistol>());
				break;
			case WeaponType::WeaponTypeUzi:
				weapon = std::static_pointer_cast<Weapon>(std::make_shared<Uzi>());
				break;
			case WeaponType::WeaponTypeNull:
				weapon = std::static_pointer_cast<Weapon>(std::make_shared<NullWeapon>());
				break;
			default:
				break;
			}
			if(weapon) 
				weapons.push_back(weapon);
		}
		else {
			std::vector<std::shared_ptr<Weapon>>::iterator it = std::find_if(weapons.begin(), weapons.end(), WeaponIsOfType(weaponType));
			if(it == weapons.end()) {
				switch(weaponType) {
				case WeaponType::WeaponTypePistol:
					weapon = std::static_pointer_cast<Weapon>(std::make_shared<Pistol>());
					break;
				case WeaponType::WeaponTypeUzi:
					weapon = std::static_pointer_cast<Weapon>(std::make_shared<Uzi>());
					break;
				case WeaponType::WeaponTypeNull:
					weapon = std::static_pointer_cast<Weapon>(std::make_shared<NullWeapon>());
					break;
				default:
					break;
				}
				if(weapon) 
					weapons.push_back(weapon);
			}
			else {
				weapon = (*it);
			}
		}
		if(weapon) weapon->AddAmmo(ammo);

}

const bool WeaponCache::IsArmed() const {
	return currentWeapon.use_count() > 0 && currentWeapon.lock()->GetType() != WeaponType::WeaponTypeNull;
}

const WeaponType WeaponCache::CurrentWeaponType() {
	return IsArmed() ? currentWeapon.lock()->GetType() : WeaponType::WeaponTypeNull;
}

void WeaponCache::UpdateSprite() {
	if(renderer.use_count() == 0) {
		renderer = GetComponent<SpriteRenderer>();
	}
	const unsigned int noOfWeapons = NumberOfWeapons();
	if(noOfWeapons >= 0 && (selectedWeapon >= 0 && selectedWeapon < noOfWeapons)) {
		currentWeapon = weapons[selectedWeapon];
		std::shared_ptr<SpriteRenderer> sr = renderer.lock();
		const sf::IntRect rect = currentWeapon.lock()->GetTextureRect();
		sr->SetTextureRect(rect.left, rect.top, rect.width, rect.height);
		sr->SetPivotManually(currentWeapon.lock()->GetImageOffset());
	}
}
