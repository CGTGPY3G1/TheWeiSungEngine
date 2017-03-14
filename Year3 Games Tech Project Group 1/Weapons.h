#pragma once
#ifndef WS_WEAPONS_H
#define WS_WEAPONS_H
#include <string>
#include "Vector2.h"
#include <SFML\Graphics\Rect.hpp>
#include <memory>


enum WeaponType {
	WeaponTypeNull = 0,
	WeaponTypePistol = 1,
	WeaponTypeUzi = 2,
	WeaponTypeNumberOfWeaponTypes
};

class Weapon {
	friend class WeaponCache;
public:
	Weapon();
	~Weapon();
	virtual const sf::IntRect GetTextureRect() = 0;
	virtual const WeaponType GetType() const = 0;
	virtual const float GetReloadTime()const = 0;
	virtual const float GetFireRate() const = 0;
	virtual const Vector2 GetImageOffset() const = 0;
	virtual const unsigned int GetRoundsPerClip() const = 0;
	virtual const float DistanceToFirePoint() const = 0;
	virtual void UpdateTimers(const float & delta);
	virtual void Fire();
	virtual void Reload();
	void AddAmmo(const int & amount);
	const bool IsReloading() const;
	const bool NeedsReloading() const;
	const bool CanFire() const;
	const unsigned int GetAmmoInCache() const;
	const unsigned int GetAmmoInClip() const;
	void Init();
	bool operator == (const Weapon & other);
protected:	
	float reloadTime, timeTilFire;
	bool reloading = true;
	Vector2 offSet;
	unsigned int ammoInCache = 0, ammoInClip = 0;
	bool enabled = true;
};

class NullWeapon : public Weapon {
public:
	NullWeapon();
	~NullWeapon();
	const sf::IntRect GetTextureRect() override;
	const WeaponType GetType() const override;
	const float GetFireRate() const override;
	const float GetReloadTime() const override;
	const Vector2 GetImageOffset() const override;
	const float DistanceToFirePoint() const override;
	const unsigned int GetRoundsPerClip() const override;
};

class Pistol : public Weapon {
public:
	Pistol();
	~Pistol();
	const sf::IntRect GetTextureRect() override;
	const WeaponType GetType() const override;
	const float GetFireRate() const override;
	const float GetReloadTime() const override;
	const Vector2 GetImageOffset() const override;
	const float DistanceToFirePoint() const override;
	const unsigned int GetRoundsPerClip() const override;
private:

};

class Uzi : public Weapon {
public:
	Uzi();
	~Uzi();
	const sf::IntRect GetTextureRect() override;
	const WeaponType GetType() const override;
	const float GetFireRate() const override;
	const float GetReloadTime() const override;
	const Vector2 GetImageOffset() const override;
	const float DistanceToFirePoint() const override;
	const unsigned int GetRoundsPerClip() const override;
};

struct WeaponIsOfType {
	WeaponIsOfType(const WeaponType & weaponType) : type(weaponType) {}
	bool operator()(const std::shared_ptr<Weapon> & other) { return other->GetType() == type; }
private:
	unsigned int type;
};

#endif // !WS_WEAPONS_H