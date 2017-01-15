#include "CameraFollowScript.h"
#include "Transform2D.h"
#include "RigidBody2D.h"
#include "GameObject.h"

CameraFollowScript::CameraFollowScript() : ScriptableComponent() {
}

CameraFollowScript::CameraFollowScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

CameraFollowScript::~CameraFollowScript() {
}

void CameraFollowScript::Start() {
	myTransform = GetComponent<Transform2D>();
}

void CameraFollowScript::Update(const float & deltaTime) {
	if(!myTarget.expired()) {
		if(!myTransform.expired()) {
			std::shared_ptr<Transform2D> transform = myTransform.lock();
			std::shared_ptr<RigidBody2D> target = myTarget.lock();
			Vector2 position = transform->GetPosition();
			transform->Move((target->GetPosition() - position) * (deltaTime * 1.5f));
			const float maxVelocity = Physics::PIXELS_PER_METRE * 2.0f;
			const float speed = target->GetSpeed();
			float oldZoom = GetZoom();
			float newZoom = (speed / maxVelocity) * 0.4f;
			SetZoom(std::max<float>(1.0f, (oldZoom * (1.0f - deltaTime) + ((1.0f + newZoom) * deltaTime))));
			view.setCenter(position);
		}
	}
}

void CameraFollowScript::SetZoom(const float & zoom) {
	const float toZoom = (zoom / zoomLevel);
	view.zoom(toZoom);
	zoomLevel = zoom;
}

float CameraFollowScript::GetZoom() {
	return zoomLevel;
}

void CameraFollowScript::SetTarget(std::weak_ptr<RigidBody2D> target) {
	myTarget = target;
}
