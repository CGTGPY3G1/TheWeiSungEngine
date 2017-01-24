#include "BulletScript.h"
#include "GameObject.h"
BulletScript::BulletScript() : ScriptableComponent() {
}
BulletScript::BulletScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {

}
BulletScript::~BulletScript() {
}
void BulletScript::Start() {
	lifeTime = 3.5f;
}
void BulletScript::Update(const float & deltaTime) {
	if(lifeTime > 0.0f) lifeTime -= deltaTime;
	else gameObject.lock()->Destroy();
}

void BulletScript::OnCollisionEnter(const CollisionData & data) {
	gameObject.lock()->Destroy();
}
