
#ifndef WS_SPRITE_RENDERING_SYSTEM_H
#define WS_SPRITE_RENDERING_SYSTEM_H
#include "System.h"
#include "SpriteBatch.h"
namespace WeiSungEngine {
	class SpriteRenderingSystem : public System {
	public:
		SpriteRenderingSystem();
		~SpriteRenderingSystem();
		void ProcessComponents(std::vector<std::shared_ptr<GameObject>> & gameObjects) override;
		const unsigned int GetComponentMask() override;
	private:
		SpriteBatch spriteBatch;
	};
}

#endif // !WS_SPRITE_RENDERING_SYSTEM_H
