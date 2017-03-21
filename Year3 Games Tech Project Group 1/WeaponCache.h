#pragma once
#ifndef WS_WEAPON_CACHE_H
#define WS_WEAPON_CACHE_H
#include "ScriptableComponent.h"
#include "Weapons.h"

class CharacterScript;
class Transform2D;
class SpriteRenderer;
class WeaponCache : public ScriptableComponent {
public:
	
	WeaponCache();
	WeaponCache(std::weak_ptr<GameObject> gameObject);
	~WeaponCache();
	void Start() override;
	void Update(const float & deltaTime) override;
	const ComponentType Type() const override { return COMPONENT_WEAPON_CACHE; }
	const std::string GetName() const override { return "WeaponCache"; }
	int GetSortOrder() override;
	void Fire();
	void AddAmmo(const WeaponType & weaponType, const int & amount);
	std::weak_ptr<Weapon> GetCurrentWeapon();
	const unsigned int NumberOfWeapons() const;
	void SwitchWeapon(const bool & forward);
	void AddWeapon(const WeaponType & weaponType, const int & ammo);
	const bool IsArmed() const;
	const WeaponType CurrentWeaponType();
	void SetShooter(std::weak_ptr<CharacterScript> shooter);
private:
	void UpdateSprite();
	std::weak_ptr<Weapon> currentWeapon;
	std::weak_ptr<CharacterScript> shooter;
	std::weak_ptr<SpriteRenderer> renderer;
	std::weak_ptr<Transform2D> transform;
	std::vector<std::shared_ptr<Weapon>> weapons;
	int selectedWeapon = 0;
	std::string shooterName = "";
};
#endif // !WS_WEAPON_CACHE_H
