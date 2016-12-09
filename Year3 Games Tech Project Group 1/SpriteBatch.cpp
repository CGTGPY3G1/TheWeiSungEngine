#include "SpriteBatch.h"
#include "Engine.h"
#include "Graphics.h"
#include <SFML/Graphics.hpp>
SpriteBatch::SpriteBatch() {
}

SpriteBatch::~SpriteBatch() {
}

void SpriteBatch::Draw(WSSprite & sprite, RenderLayer layer, const int & sortOrder) {
	
}

void SpriteBatch::Draw(WSSprite sprite, const sf::RenderStates & states, RenderLayer layer, const int & sortOrder) {
	int layerIndex = (unsigned int)layer;
	if(layerIndex >= layers.size()) layers.resize((unsigned int)(layer + 1));
	SpriteContainer s(sprite, states, sortOrder);
	layers[layerIndex].sprites.push_back(s);
}

void SpriteBatch::Clear() {
	layers.clear();
}

void SpriteBatch::Display() {
	std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
	Sort();
	for(size_t i = 0; i < layers.size(); i++) {
		Batch batch = layers[i];
		for(std::vector<SpriteContainer>::iterator i = batch.sprites.begin(); i != batch.sprites.end(); ++i) {
			//sf::Transform t = (*i).m_states.transform;
			graphics->GetWindow().draw((*i).m_sprite, (*i).m_states);
		}
	}
}
