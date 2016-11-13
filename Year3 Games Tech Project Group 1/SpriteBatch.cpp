#include "SpriteBatch.h"
#include "Engine.h"
#include "Graphics.h"
#include <SFML/Graphics.hpp>
SpriteBatch::SpriteBatch() {
}

SpriteBatch::~SpriteBatch() {
}

void SpriteBatch::Draw(WSSprite & sprite, RenderLayer layer, const int & sortOrder) {
	int layerIndex = (unsigned int)layer;
	if(layerIndex >= layers.size()) layers.resize((unsigned int)(layer + 1));
	layers[layerIndex].sprites.push_back(SpriteContainer(sprite, sortOrder));
}

void SpriteBatch::Draw(WSSprite sprite, sf::Transformable transformable, RenderLayer layer, const int & sortOrder) {
	sprite.move(transformable.getPosition());
	sprite.rotate(transformable.getRotation());
	sprite.scale(transformable.getScale());
	Draw(sprite, layer, sortOrder);
}

void SpriteBatch::Clear() {
	layers.clear();
}

void SpriteBatch::Display() {
	Graphics * graphics = Engine::GetInstance().GetGraphics();
	Sort();
	for(size_t i = 0; i < layers.size(); i++) {
		Batch batch = layers[i];
		for(std::vector<SpriteContainer>::iterator i = batch.sprites.begin(); i != batch.sprites.end(); ++i) {
			graphics->GetWindow().draw((*i).m_sprite);
		}
	}
}
