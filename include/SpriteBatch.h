
#ifndef WS_SPRITE_BATCH_H
#define WS_SPRITE_BATCH_H
#include "WSSprite.h"
#include "RenderLayer.h"
#include <vector>
namespace WeiSungEngine {
	struct SpriteContainer {
		SpriteContainer() {}
		SpriteContainer(WSSprite sprite, const sf::RenderStates & states, const int & sortOrder, const unsigned int & id) : m_sprite(sprite), m_states(states), m_sortOrder(sortOrder), m_id(id) {}
		bool operator <(const SpriteContainer & other) { return m_sortOrder < other.m_sortOrder || (m_sortOrder == other.m_sortOrder && m_id < other.m_id); }
		sf::RenderStates m_states;
		WSSprite m_sprite;
		int m_sortOrder;
		unsigned int m_id;
	};

	struct Batch {
		Batch() {}
		Batch(const unsigned int & renderLayer, const sf::RenderStates & states) : m_renderLayer(renderLayer), rStates(states) {}
		bool operator <(const Batch & other) { return m_renderLayer < other.m_renderLayer; }
		sf::RenderStates rStates;
		std::vector<SpriteContainer> sprites;
		unsigned int m_renderLayer;
		void Sortit() { std::sort(sprites.begin(), sprites.end()); }
	};

	struct DrawLayer {
		std::vector<Batch> batches;
		void Sort() {
			for (std::vector<Batch>::iterator i = batches.begin(); i != batches.end(); ++i) (*i).Sortit();
			std::sort(batches.begin(), batches.end());
		}
	};
	class SpriteBatch {
	public:
		SpriteBatch();
		~SpriteBatch();
		void Draw(WSSprite sprite, const sf::RenderStates & states, RenderLayer layer = RenderLayer::MIDGROUND_LAYER, const int & sortOrder = 0, const unsigned int & id = 0);
		void Clear();
		void Display();
	private:
		std::vector<Batch> layers;
		std::vector<sf::VertexArray>vertexArrays;
		void Sort() {
			for (std::vector<Batch>::iterator i = layers.begin(); i != layers.end(); ++i) (*i).Sortit();
			std::sort(layers.begin(), layers.end());
		}
	};
}
#endif // !WS_SPRITE_BATCH_H
