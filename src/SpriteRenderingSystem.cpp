#include "SpriteRenderingSystem.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Engine.h"
#include "Graphics.h"
namespace WeiSungEngine {
	SpriteRenderingSystem::SpriteRenderingSystem() : System() {
	}

	SpriteRenderingSystem::~SpriteRenderingSystem() {
	}

	void SpriteRenderingSystem::ProcessComponents(std::vector<std::shared_ptr<GameObject>> & gameObjects) {
		const unsigned int mask = GetComponentMask();
		const size_t noOfObjects = gameObjects.size();
		spriteBatch.Clear();
		const std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
		const sf::Vector2f size = graphics->GetView().getSize(), position = graphics->GetView().getCenter() - (size * 0.5f);
		const sf::FloatRect viewRect = sf::FloatRect(position, size);
		for (size_t i = 0; i < noOfObjects; i++) {
			std::shared_ptr<GameObject> go = gameObjects[i];
			if (go) {
				if (go->GetEnabled()) {
					if (go->HasComponents(mask)) {
						std::shared_ptr<Transform2D> transform = go->GetComponent<Transform2D>().lock();
						std::vector<std::weak_ptr<SpriteRenderer>> spriteRenderers = go->GetComponents<SpriteRenderer>();
						for (std::vector<std::weak_ptr<SpriteRenderer>>::iterator i = spriteRenderers.begin(); i != spriteRenderers.end(); ++i) {
							std::shared_ptr<SpriteRenderer> renderer = (*i).lock();
							if (renderer && renderer->GetEnabled()) {
								sf::RenderStates states;
								states.shader = renderer->GetShader();
								states.transform = transform->GetWorldTransform();
								const WSSprite sprite = renderer->GetSprite();
								const sf::FloatRect spriteRect = states.transform.transformRect(sprite.getGlobalBounds());
								if (viewRect.intersects(spriteRect)) spriteBatch.Draw(sprite, states, renderer->GetSortLayer(), renderer->GetSortOrder(), renderer->GetCompID());
							}
						}
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
}
