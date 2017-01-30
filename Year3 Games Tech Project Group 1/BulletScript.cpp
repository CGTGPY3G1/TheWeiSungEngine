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
	else {
		if(!GetComponent<AudioSource>().lock()->IsPlaying()) gameObject.lock()->Destroy();
		else if(alive) {
			GetComponent<RigidBody2D>().lock()->SetEnabled(false);
			GetComponent<SpriteRenderer>().lock()->SetEnabled(false);
			alive = false;
		}
	}
}

void BulletScript::OnCollisionEnter(const CollisionData & data) {
	lifeTime = -1.0f;
	
}

int BulletScript::GetSortOrder() {
	return TypeInfo::ScriptSortOrder<BulletScript>();
}
