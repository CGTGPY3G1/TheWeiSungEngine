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
	template<> const static ComponentType GetTypeID<RevoluteJoint>() { return COMPONENT_REVOLUTE_JOINT; }
	template<> const static ComponentType GetTypeID<CharacterMovementScript>() { return COMPONENT_CHARACTER_MOVEMENT; }

	template <typename T> const static bool IsComponent() { return false; };
	template<> const static bool IsComponent<Component>() { return true; }
	template<> const static bool IsComponent<Transform2D>() { return true; }
	template<> const static bool IsComponent<SpriteRenderer>() { return true; }
	template<> const static bool IsComponent<RigidBody2D>() { return true; }
	template<> const static bool IsComponent<BoxCollider>() { return true; }
	template<> const static bool IsComponent<CircleCollider>() { return true; }
	template<> const static bool IsComponent<PolygonCollider>() { return true; }
	template<> const static bool IsComponent<WheelJoint>() { return true; }
	template<> const static bool IsComponent<RevoluteJoint>() { return true; }
	template<> const static bool IsComponent<CharacterMovementScript>() { return true; }

	template <typename T> const static bool AllowMultiple() { return false; };
	template<> const static bool AllowMultiple<SpriteRenderer>() { return true; }
	template<> const static bool AllowMultiple<BoxCollider>() { return true; }
	template<> const static bool AllowMultiple<CircleCollider>() { return true; }
	template<> const static bool AllowMultiple<PolygonCollider>() { return true; }
	template<> const static bool AllowMultiple<WheelJoint>() { return true; }
	template<> const static bool AllowMultiple<RevoluteJoint>() { return true; }

	template <typename T> const static bool IsScriptable() { return false; }; // to be used for scriptable components
	template<> const static bool IsScriptable<CharacterMovementScript>() { return true; }


	template <typename T> const static bool IsCollider() { return false; };
	template<> const static bool IsCollider<BoxCollider>() { return true; }
	template<> const static bool IsCollider<CircleCollider>() { return true; }
	template<> const static bool IsCollider<PolygonCollider>() { return true; }

	const static bool IsCollider(const ComponentType & type) { 
		static const int colliderMask = COMPONENT_BOX_COLLIDER_2D | COMPONENT_CIRCLE_COLLIDER | COMPONENT_POLYGON_COLLIDER_2D;
		return (colliderMask & type) == type;
	};
};
#endif // !WS_TYPE_INFO_H