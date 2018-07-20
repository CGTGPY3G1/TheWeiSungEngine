#include "SpriteBatch.h"
#include "Engine.h"
#include "Graphics.h"
#include <SFML/Graphics.hpp>
namespace WeiSungEngine {
	SpriteBatch::SpriteBatch() {
	}

	SpriteBatch::~SpriteBatch() {
	}

	void SpriteBatch::Draw(WSSprite sprite, const sf::RenderStates & states, RenderLayer layer, const int & sortOrder, const unsigned int & id) {
		int layerIndex = (unsigned int)layer;
		if (layerIndex >= (int)layers.size()) layers.resize((unsigned int)(layer + 1));
		layers[layerIndex].sprites.push_back(SpriteContainer(sprite, states, sortOrder, id));
	}

	void SpriteBatch::Clear() {
		layers.clear();
	}

	void SpriteBatch::Display() {
		const std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
		Sort();
		for (size_t i = 0; i < layers.size(); i++) {
			Batch batch = layers[i];
			for (std::vector<SpriteContainer>::iterator it = batch.sprites.begin(); it != batch.sprites.end(); ++it) {
				SpriteContainer sc = (*it);
				graphics->Draw(sc.m_sprite, sc.m_states);
			}
		}
	}
}
