#include "SpriteRenderingSystem.h"

#include "SpriteRenderer.h"
#include "GameObject.h"
SpriteRenderingSystem::SpriteRenderingSystem() {
}

SpriteRenderingSystem::~SpriteRenderingSystem() {
}

void SpriteRenderingSystem::ProcessComponents(std::vector<std::shared_ptr<GameObject>> & gameObjects) {
	unsigned int mask = GetComponentMask();
	unsigned int noOfObjects = gameObjects.size();
	spriteBatch.Clear();
	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<GameObject> go = gameObjects[i];
		if(go) {
			if(go->HasComponents(mask)) {
				std::shared_ptr<Transform2D> transform = go->GetComponent<Transform2D>().lock();
				std::vector<std::weak_ptr<SpriteRenderer>> spriteRenderers = go->GetComponents<SpriteRenderer>();
				for(std::vector<std::weak_ptr<SpriteRenderer>>::iterator i = spriteRenderers.begin(); i != spriteRenderers.end(); ++i) {
					std::shared_ptr<SpriteRenderer> renderer = (*i).lock();
					if(renderer && renderer->GetEnabled()) spriteBatch.Draw(renderer->GetSprite(), transform->GetWorldTransformable(), renderer->GetSortLayer(), renderer->GetSortOrder());
				}			
			}
		}
	}
	spriteBatch.Display();
}

const unsigned int SpriteRenderingSystem::GetComponentMask() {
	static const unsigned int mask(ComponentType::COMPONENT_TRANSFORM_2D | ComponentType::COMPONENT_SPRITE_RENDERER);
	return mask;
}
