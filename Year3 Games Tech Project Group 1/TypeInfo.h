#pragma once
#ifndef WS_TYPE_INFO_H
#define WS_TYPE_INFO_H

#include "ComponentHeaders.h"

class TypeInfo {
public:
	template <typename T> const static ComponentType GetTypeID() {
		return ComponentType::COMPONENT_NULL;
	};
	template<> const static ComponentType GetTypeID<Transform2D>() { return COMPONENT_TRANSFORM_2D; }
	template<> const static ComponentType GetTypeID<SpriteRenderer>() { return COMPONENT_SPRITE_RENDERER; }
	template<> const static ComponentType GetTypeID<RigidBody2D>() { return COMPONENT_RIGID_BODY_2D; }
	template<> const static ComponentType GetTypeID<BoxCollider>() { return COMPONENT_BOX_COLLIDER_2D; }
	template<> const static ComponentType GetTypeID<CircleCollider>() { return COMPONENT_CIRCLE_COLLIDER; }
	template<> const static ComponentType GetTypeID<PolygonCollider>() { return COMPONENT_POLYGON_COLLIDER_2D; }

	template <typename T> const static bool IsComponent() { return false; };
	template<> const static bool IsComponent<Component>() { return true; }
	template<> const static bool IsComponent<Transform2D>() { return true; }
	template<> const static bool IsComponent<SpriteRenderer>() { return true; }
	template<> const static bool IsComponent<RigidBody2D>() { return true; }
	template<> const static bool IsComponent<BoxCollider>() { return true; }
	template<> const static bool IsComponent<CircleCollider>() { return true; }
	template<> const static bool IsComponent<PolygonCollider>() { return true; }

	template <typename T> const static bool AllowMultiple() { return false; };
	template<> const static bool AllowMultiple<Transform2D>() { return false; }
	template<> const static bool AllowMultiple<SpriteRenderer>() { return true; }
	template<> const static bool AllowMultiple<RigidBody2D>() { return false; }
	template<> const static bool AllowMultiple<BoxCollider>() { return true; }
	template<> const static bool AllowMultiple<CircleCollider>() { return true; }
	template<> const static bool AllowMultiple<PolygonCollider>() { return true; }

	template <typename T> const static bool IsScriptable() { return false; }; // to be used for scriptable components

	template <typename T> const static bool IsCollider() { return false; };
	template<> const static bool IsCollider<BoxCollider>() { return true; }
	template<> const static bool IsCollider<CircleCollider>() { return true; }
	template<> const static bool IsCollider<PolygonCollider>() { return true; }
};
#endif // !WS_TYPE_INFO_H