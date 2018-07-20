
#ifndef WS_SPRITE_ANIMATOR_H
#define WS_SPRITE_ANIMATOR_H
#include "ScriptableComponent.h"
namespace WeiSungEngine {
	struct AnimationFrame {
		int x, y, width, height;
		float frameTime;
		AnimationFrame(const float & time = 1.0f, const int & rectX = 0, const int & rectY = 0, const int & rectWidth = 32, const int & rectHeight = 32) : frameTime(time), x(rectX), y(rectY), width(rectWidth), height(rectHeight) {}
	};

	struct Animation {
		std::string name;
		int currentFrameIndex = 0;
		float timeTilSwitch = 0.2f;
		bool looping = true;
		bool playing = false;
		std::vector<AnimationFrame> frames;
		Animation(const std::string animName = "Null Anim") : name(animName) {}
		void AddFrame(const AnimationFrame & frame) { frames.push_back(frame); }
		bool Update(const float & delta) {
			bool toReturn = false;
			const int numberOfFrames = (int)frames.size();

			if (currentFrameIndex >= 0 && currentFrameIndex < numberOfFrames) {
				if (playing) {
					timeTilSwitch -= delta;
					while (timeTilSwitch <= 0.0f) {
						if (looping) {
							currentFrameIndex++;
							toReturn = true;
							if (currentFrameIndex >= numberOfFrames) {
								currentFrameIndex = 0;
							}
						}
						else {
							if (currentFrameIndex < numberOfFrames - 1) {
								currentFrameIndex++;
								toReturn = true;
							}
							else playing = false;
						}
						timeTilSwitch += frames[currentFrameIndex].frameTime;
					}
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
		const bool IsLooping() const;
		const bool IsPlaying() const;
		void SetPlaying(const bool & playing);
	private:
		void UpdateRenderer();
		std::weak_ptr<SpriteRenderer> renderer;
		std::vector<Animation> anims;
		Animation currentAnim;
	};
}
#endif // !WS_SPRITE_ANIMATOR_H
