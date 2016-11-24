#pragma once
#ifndef WS_COMPONENT_DATA_H
#define WS_COMPONENT_DATA_H

#include "TypeInfo.h"

struct ComponentData {
	ComponentType type;
	std::weak_ptr<Component> comp;
};

#endif // !WS_COMPONENT_DATA_H