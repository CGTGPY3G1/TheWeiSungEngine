#pragma once
#ifndef WS_TYPE_INFO_H
#define WS_TYPE_INFO_H

#include "ComponentHeaders.h"

class TypeInfo {
public:
	template <typename T> const static ComponentType GetTypeID() { return ComponentType::COMPONENT_NULL; };
	template<> const static ComponentType GetTypeID<Transform2D>() { return COMPONENT_TRANSFORM_2D; }
	template<> const static ComponentType GetTypeID<SpriteRenderer>() { return COMPONENT_SPRITE_RENDERER; }
	template<> const static ComponentType GetTypeID<RigidBody2D>() { return COMPONENT_RIGID_BODY_2D; }
	template<> const static ComponentType GetTypeID<BoxCollider>() { return COMPONENT_BOX_COLLIDER_2D; }
	template<> const static ComponentType GetTypeID<CircleCollider>() { return COMPONENT_CIRCLE_COLLIDER; }
	template<> const static ComponentType GetTypeID<PolygonCollider>() { return COMPONENT_POLYGON_COLLIDER_2D; }
	template<> const static ComponentType GetTypeID<WheelJoint>() { return COMPONENT_WHEEL_JOINT; }
	template<> const static ComponentType GetTypeID<AttackerIdentityScript>() { return COMPONENT_ATTACKER_IDENTITY_SCRIPT; }
	template<> const static ComponentType GetTypeID<CharacterScript>() { return COMPONENT_CHARACTER_SCRIPT; }
	template<> const static ComponentType GetTypeID<CameraFollowScript>() { return COMPONENT_CAMERA_FOLLOW; }
	template<> const static ComponentType GetTypeID<CivWaypointScript>() { return COMPONENT_CIV_WAYPOINT; }
	template<> const static ComponentType GetTypeID<TileMapper>() { return COMPONENT_TILE_MAPPER; }
	template<> const static ComponentType GetTypeID<VehicleController>() { return COMPONENT_VEHICLE_CONTROLLER; }
	template<> const static ComponentType GetTypeID<BulletScript>() { return COMPONENT_BULLET_SCRIPT; }
	template<> const static ComponentType GetTypeID<PlayerScript>() { return COMPONENT_PLAYER_SCRIPT; }
	template<> const static ComponentType GetTypeID<AudioSource>() { return COMPONENT_AUDIO_SOURCE; }
	template<> const static ComponentType GetTypeID<HealthScript>() { return COMPONENT_HEALTH_SCRIPT; }
	template<> const static ComponentType GetTypeID<DeathTimer>() { return COMPONENT_DEATH_TIMER; }
	template<> const static ComponentType GetTypeID<SpriteAnimator>() { return COMPONENT_SPRITE_ANIMATOR; }
	template<> const static ComponentType GetTypeID<BloodSplatterScript>() { return COMPONENT_BLOOD_SPLATTER_SCRIPT; }
	template<> const static ComponentType GetTypeID<WeaponCache>() { return COMPONENT_WEAPON_CACHE; }
	template<> const static ComponentType GetTypeID<DamageScript>() { return COMPONENT_DAMAGE_SCRIPT; }
	template<> const static ComponentType GetTypeID<GrenadeScript>() { return COMPONENT_GRENADE_SCRIPT; }
	template<> const static ComponentType GetTypeID<SelfDestructingAnimScript>() { return COMPONENT_SELF_DESTRUCTING_ANIM_SCRIPT; }

	template <typename T> const static bool AllowMultiple() { return false; };
	template<> const static bool AllowMultiple<SpriteRenderer>() { return true; }
	template<> const static bool AllowMultiple<BoxCollider>() { return true; }
	template<> const static bool AllowMultiple<CircleCollider>() { return true; }
	template<> const static bool AllowMultiple<PolygonCollider>() { return true; }
	template<> const static bool AllowMultiple<WheelJoint>() { return true; }
	template<> const static bool AllowMultiple<TileMapper>() { return true; }
	template<> const static bool AllowMultiple<AudioSource>() { return true; }

	template <typename T> const static bool IsScriptable() { return false; };
	template<> const static bool IsScriptable<CharacterScript>() { return true; }
	template<> const static bool IsScriptable<CameraFollowScript>() { return true; }
	template<> const static bool IsScriptable<CivWaypointScript>() { return true; }
	template<> const static bool IsScriptable<VehicleController>() { return true; }
	template<> const static bool IsScriptable<BulletScript>() { return true; }
	template<> const static bool IsScriptable<PlayerScript>() { return true; }
	template<> const static bool IsScriptable<AttackerIdentityScript>() { return true; }
	template<> const static bool IsScriptable<HealthScript>() { return true; }
	template<> const static bool IsScriptable<DeathTimer>() { return true; }
	template<> const static bool IsScriptable<SpriteAnimator>() { return true; }
	template<> const static bool IsScriptable<BloodSplatterScript>() { return true; }
	template<> const static bool IsScriptable<WeaponCache>() { return true; }
	template<> const static bool IsScriptable<DamageScript>() { return true; }
	template<> const static bool IsScriptable<GrenadeScript>() { return true; }
	template<> const static bool IsScriptable<SelfDestructingAnimScript>() { return true; }

	template <typename T> const static int ScriptSortOrder() { return 33; };
	template<> const static int ScriptSortOrder<PlayerScript>() { return 0; }
	template<> const static int ScriptSortOrder<CharacterScript>() { return 1; }
	template<> const static int ScriptSortOrder<VehicleController>() { return 2; }
	template<> const static int ScriptSortOrder<CivWaypointScript>() { return 3; }	
	template<> const static int ScriptSortOrder<CameraFollowScript>() { return 4; }
	template<> const static int ScriptSortOrder<AttackerIdentityScript>() { return 5; }
	template<> const static int ScriptSortOrder<BulletScript>() { return 6; }
	template<> const static int ScriptSortOrder<HealthScript>() { return 7; }
	template<> const static int ScriptSortOrder<SpriteAnimator>() { return 8; }
	template<> const static int ScriptSortOrder<BloodSplatterScript>() { return 9; }
	template<> const static int ScriptSortOrder<WeaponCache>() { return 10; }
	template<> const static int ScriptSortOrder<DamageScript>() { return 11; }
	template<> const static int ScriptSortOrder<GrenadeScript>() { return 12; };
	template<> const static int ScriptSortOrder<SelfDestructingAnimScript>() { return 13; };
	template<> const static int ScriptSortOrder<DeathTimer>() { return 55; }
	
	template <typename T> const static bool IsCollider() { return false; };
	template<> const static bool IsCollider<BoxCollider>() { return true; }
	template<> const static bool IsCollider<CircleCollider>() { return true; }
	template<> const static bool IsCollider<PolygonCollider>() { return true; }

	const static bool IsUpdatable(const ComponentType & type) {
		static const int mask = COMPONENT_BLOOD_SPLATTER_SCRIPT | COMPONENT_SPRITE_ANIMATOR | COMPONENT_DEATH_TIMER | COMPONENT_BULLET_SCRIPT | COMPONENT_CAMERA_FOLLOW |
			COMPONENT_PLAYER_SCRIPT | COMPONENT_HEALTH_SCRIPT | COMPONENT_WEAPON_CACHE | COMPONENT_GRENADE_SCRIPT| COMPONENT_SELF_DESTRUCTING_ANIM_SCRIPT;
		return ((mask & type) == type);
	};
	
	const static bool IsFixedUpdatable(const ComponentType & type) {
		static const int mask = COMPONENT_CHARACTER_SCRIPT | COMPONENT_CIV_WAYPOINT | 
			COMPONENT_PLAYER_SCRIPT | COMPONENT_VEHICLE_CONTROLLER;
		return ((mask & type) == type);
	};

	const static bool IsLateUpdatable(const ComponentType & type) {
		/*static const int colliderMask = COMPONENT_SELF_DESTRUCTING_ANIM_SCRIPT;
		return (colliderMask & type) == type;*/
		return false;
	};

	const static bool IsRenderable(const ComponentType & type) {
		static const int mask = COMPONENT_SPRITE_RENDERER;// | COMPONENT_VEHICLE_CONTROLLER;
		return ((mask & type) == type);
	};

	const static bool IsCollider(const ComponentType & type) { 
		static const int mask = COMPONENT_BOX_COLLIDER_2D | COMPONENT_CIRCLE_COLLIDER | COMPONENT_POLYGON_COLLIDER_2D;
		return ((mask & type) == type);
	};
};
#endif // !WS_TYPE_INFO_H