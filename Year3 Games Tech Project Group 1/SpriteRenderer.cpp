#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer() : Component() {
}

SpriteRenderer::SpriteRenderer(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {
}

SpriteRenderer::~SpriteRenderer() {

}

int SpriteRenderer::GetSortOrder() {
	return sortOrder;
}

void SpriteRenderer::SetSortOrder(const int & newOrder) {
	sortOrder = newOrder;
}

int SpriteRenderer::GetSortLayer() {
	return sortLayer;
}

void SpriteRenderer::SetSortLrder(const int & newLayer) {
	sortLayer = newLayer;
}

float SpriteRenderer::GetPixelsPeMetre() {
	return pixelsPerMeter;
}

void SpriteRenderer::SetPixelsPeMetre(const float & PPM) {
	pixelsPerMeter = PPM;
}

sf::Sprite & SpriteRenderer::GetSprite() {
	return sprite;
}

void SpriteRenderer::SetSprite(const sf::Sprite & sprite) {
}

void SpriteRenderer::LoadSprite(const std::string path) {
	////  Do this
}
