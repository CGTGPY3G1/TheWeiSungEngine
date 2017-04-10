#include "SelfDestructingAnimScript.h"
#include "GameObject.h"
#include "Transform2D.h"
#include "AudioSource.h"
#include "SpriteAnimator.h"
#include "TypeInfo.h"

SelfDestructingAnimScript::SelfDestructingAnimScript() {
}

SelfDestructingAnimScript::SelfDestructingAnimScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

SelfDestructingAnimScript::~SelfDestructingAnimScript() {
}

void SelfDestructingAnimScript::Start() {
	audioSource = GetComponent<AudioSource>();
	spriteAnimator = GetComponent<SpriteAnimator>();
	transform = GetComponent<Transform2D>();
}

void SelfDestructingAnimScript::Update(const float & deltaTime) {
	if(alive) {
		bool stillAlive = false;
		if(spriteAnimator.use_count() > 0) {
			stillAlive = spriteAnimator.lock()->IsPlaying();
		}
		if(!stillAlive && audioSource.use_count() > 0) {
			stillAlive = audioSource.lock()->IsPlaying();
		}
		if(stillAlive) {
			transform.lock()->Scale(1.0f + deltaTime);
		}
		else {
			SelfDestruct();
		}
	}
}

int SelfDestructingAnimScript::GetSortOrder() {
	return TypeInfo::ScriptSortOrder<SelfDestructingAnimScript>();
}

void SelfDestructingAnimScript::SetAnimator(const std::weak_ptr<SpriteAnimator>& animator) {
	spriteAnimator = animator;
}

void SelfDestructingAnimScript::SetAudioSource(const std::weak_ptr<AudioSource>& source) {
	audioSource = source;
}

void SelfDestructingAnimScript::SelfDestruct() {
	if(alive) {
		alive = false;
		gameObject.lock()->Destroy();
	}
}
