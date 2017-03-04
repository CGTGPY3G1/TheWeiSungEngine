#include "DeathTimer.h"
#include "TypeInfo.h"
#include "GameObject.h"

DeathTimer::DeathTimer() : ScriptableComponent() {
}

DeathTimer::DeathTimer(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

DeathTimer::~DeathTimer() {
}

void DeathTimer::SetTime(const float & time) {
	lifeTime = time;
}

float DeathTimer::GetTime() {
	return lifeTime;
}

void DeathTimer::Update(const float & deltaTime) {
	if(lifeTime > 0.0f) lifeTime -= deltaTime;
	else {
		GetGameObject().lock()->Destroy();
	}
}

int DeathTimer::GetSortOrder() {
	const static int order = TypeInfo::ScriptSortOrder<DeathTimer>();
	return order;
}
