#pragma once
#ifndef WS_SELF_DESTRUCTING_ANIM_SCRIPT_H
#define WS_SELF_DESTRUCTING_ANIM_SCRIPT_H
#include "ScriptableComponent.h"
class SpriteAnimator;
class AudioSource;
class Transform2D;
class SelfDestructingAnimScript : public ScriptableComponent {
public:
	SelfDestructingAnimScript();
	SelfDestructingAnimScript(std::weak_ptr<GameObject> gameObject);
	~SelfDestructingAnimScript();
	const ComponentType Type() const override { return COMPONENT_SELF_DESTRUCTING_ANIM_SCRIPT; }
	void Start() override;
	void Update(const float & deltaTime) override;
	
	const std::string GetName() const override { return "SelfDestructingAnimScript"; }
	int GetSortOrder() override;
	void SetAnimator(const std::weak_ptr<SpriteAnimator> & animator);
	void SetAudioSource(const std::weak_ptr<AudioSource> & source);
private:
	void SelfDestruct();
	std::weak_ptr<Transform2D> transform;
	std::weak_ptr<SpriteAnimator> spriteAnimator;
	std::weak_ptr<AudioSource> audioSource;
	bool alive = true;
};

#endif // !WS_SELF_DESTRUCTING_ANIM_SCRIPT_H