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
				currentAnim.timeTilSwitch = currentAnim.frames[0].frameTime;
				currentAnim.playing = true;
				UpdateRenderer();
				break;
			}
		}
	}
}

const bool SpriteAnimator::IsLooping() const {
	return currentAnim.looping;
}


const bool SpriteAnimator::IsPlaying() const {
	return currentAnim.playing;
}

void SpriteAnimator::SetPlaying(const bool & playing) {
	currentAnim.playing = playing;
}

void SpriteAnimator::UpdateRenderer() {
	if(renderer.use_count() == 0) renderer = GetComponent<SpriteRenderer>();
	std::shared_ptr<SpriteRenderer> r = renderer.lock();
	if(r) {
		AnimationFrame frame = currentAnim.GetCurrentFrame();
		r->SetTextureRect(frame.x, frame.y, frame.width, frame.height);
	}
}
