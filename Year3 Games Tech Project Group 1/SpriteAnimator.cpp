#include "SpriteAnimator.h"
#include "TypeInfo.h"
#include <algorithm>
#include "GameObject.h"
SpriteAnimator::SpriteAnimator() : ScriptableComponent() {
}

SpriteAnimator::SpriteAnimator(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

SpriteAnimator::~SpriteAnimator() {
}

void SpriteAnimator::Update(const float & deltaTime) {
	if(currentAnim.Update(deltaTime)) UpdateRenderer();
}

int SpriteAnimator::GetSortOrder() {
	static const unsigned int order = TypeInfo::ScriptSortOrder<SpriteAnimator>();
	return order;
}

void SpriteAnimator::AddAnimation(const Animation & newAnim) {
	anims.push_back(newAnim);
}

void SpriteAnimator::PlayAnimation(const std::string & animName) {
	if(currentAnim.name.compare(animName) != 0) {
		for(size_t i = 0; i < anims.size(); i++) {
			if(anims[i].name.compare(animName) == 0) {
				currentAnim = anims[i];
				currentAnim.currentFrameIndex = 0;
				currentAnim.timeTilSwitch = currentAnim.frames[currentAnim.currentFrameIndex].frameTime;
				UpdateRenderer();
				break;
			}
		}
	}
	
}

void SpriteAnimator::UpdateRenderer() {
	if(renderer.use_count() == 0) renderer = GetComponent<SpriteRenderer>();
	std::shared_ptr<SpriteRenderer> r = renderer.lock();
	if(r) {
		AnimationFrame frame = currentAnim.GetCurrentFrame();
		r->SetTextureRect(frame.x, frame.y, frame.width, frame.height);
	}
}
