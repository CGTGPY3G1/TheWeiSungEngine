#pragma once
#ifndef WS_BULLET_SCRIPT_H
#define WS_BULLET_SCRIPT_H
#include "ScriptableComponent.h"

class BulletScript : public ScriptableComponent {
public:
	BulletScript();
	BulletScript(std::weak_ptr<GameObject> gameObject);
	~BulletScript();
	const ComponentType Type() const override { return COMPONENT_BULLET_SCRIPT; }
	void Start() override;
	void Update(const float & deltaTime) override;
	const std::string GetName() const override { return "BulletScript"; }
	void OnCollisionEnter(const CollisionData & data) override;
	int GetSortOrder() override;
private:
	float lifeTime = 1.5f;
	bool alive = true;
};

#endif // !WS_BULLET_SCRIPT_H