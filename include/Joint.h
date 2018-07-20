#pragma once
#ifndef WS_JOINT_COMPONENT_H
#define WS_JOINT_COMPONENT_H
#include "Component.h"
#include <Box2D\Dynamics\Joints\b2Joint.h>
namespace WeiSungEngine {
	class Joint : public Component {

	public:
		Joint();
		Joint(std::weak_ptr<GameObject> gameObject);
		virtual ~Joint();
		const std::string GetName() const override { return "Joint"; }
		template <class Archive>
		void load(Archive & ar) {

		}

		template <class Archive>
		void save(Archive & ar) const {
			Component::save(ar);
		}
	public:
		b2Joint * joint;
		b2JointDef * jointDef;
	};
}
#endif // !WS_JOINT_COMPONENT_H
