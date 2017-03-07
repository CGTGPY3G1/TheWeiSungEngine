#pragma once
#pragma once
#ifndef WS_SPRITE_ANIMATOR_H
#define WS_SPRITE_ANIMATOR_H
#include "ScriptableComponent.h"

struct AnimationFrame {
	int x, y, width, height;
	float frameTime;
	AnimationFrame(const float & time = 1.0f, const int & rectX = 0, const int & rectY = 0, const int & rectWidth = 32, const int & rectHeight = 32) : frameTime(time), x(rectX), y(rectY), width(rectWidth), height(rectHeight) {}
};

struct Animation {
	std::string name;
	int currentFrameIndex = 0;
	float timeTilSwitch = 0.2f;
	bool playInReverse = false;
	std::vector<AnimationFrame> frames;
	Animation(const std::string animName = "Null Anim") : name(animName) {}
	void AddFrame(const AnimationFrame & frame) { frames.push_back(frame); }
	bool Update(const float & delta) {
		bool toReturn = false;
		const int numberOfFrames = (int)frames.size();
		if(currentFrameIndex >= 0 && currentFrameIndex < numberOfFrames) {
			timeTilSwitch -= delta;
			while(timeTilSwitch <= 0.0f) {
				if(playInReverse) {
					currentFrameIndex--;
					if(currentFrameIndex < 0) currentFrameIndex = numberOfFrames - 1;
				}
				else {
					currentFrameIndex++;
					if(currentFrameIndex >= numberOfFrames) currentFrameIndex = 0;
				}
				timeTilSwitch += frames[currentFrameIndex].frameTime;
				if(!toReturn) toReturn = true;
			}
		}
		return toReturn;
	}

	const int GetCurrentFrameIndex() const { return currentFrameIndex; }
	void SetCurrentFrame(const int & frameIndex) { currentFrameIndex = frameIndex; }
	AnimationFrame GetCurrentFrame() const { return frames[currentFrameIndex]; }
};

class SpriteRenderer;
class SpriteAnimator : public ScriptableComponent {
public:
	SpriteAnimator();
	SpriteAnimator(std::weak_ptr<GameObject> gameObject);
	~SpriteAnimator();
	const ComponentType Type() const override { return COMPONENT_SPRITE_ANIMATOR; }
	void Update(const float & deltaTime) override;
	const std::string GetName() const override { return "SpriteAnimator"; }
	int GetSortOrder() override;
	
	void AddAnimation(const Animation & newAnim);
	void PlayAnimation(const std::string & animName);

private:
	void UpdateRenderer();
	std::weak_ptr<SpriteRenderer> renderer;
	std::vector<Animation> anims;
	Animation currentAnim;
	bool alive = true;
};

#endif // !WS_SPRITE_ANIMATOR_H