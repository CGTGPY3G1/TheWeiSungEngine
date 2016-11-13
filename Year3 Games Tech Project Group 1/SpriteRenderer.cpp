#include "SpriteRenderer.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Scene.h"

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

RenderLayer SpriteRenderer::GetSortLayer() {
	return sortLayer;
}

void SpriteRenderer::SetSortLayer(const RenderLayer & newLayer) {
	sortLayer = newLayer;
}

float SpriteRenderer::GetPixelsPeMetre() {
	return pixelsPerMeter;
}

void SpriteRenderer::SetPixelsPeMetre(const float & PPM) {
	pixelsPerMeter = PPM;
}

WSSprite & SpriteRenderer::GetSprite() {
	return sprite;
}

void SpriteRenderer::Init(const WSSprite & sprite, const RenderLayer & sortLayer, const int & sortOrder) {
	SetSprite(sprite); SetSortLayer(sortLayer); SetSortOrder(sortOrder); SetEnabled(true);
}

void SpriteRenderer::Init(const std::string path, const RenderLayer & sortLayer, const int & sortOrder) {
	LoadSprite(path); SetSortLayer(sortLayer); SetSortOrder(sortOrder); SetEnabled(true);
	sf::FloatRect bounds = sprite.getGlobalBounds();
	float centreX = bounds.width / 2, centreY = bounds.height / 2;
	sprite.setOrigin(centreX, centreY);
}

void SpriteRenderer::SetSprite(const WSSprite & sprite) {
	this->sprite = sprite;
}

void SpriteRenderer::LoadSprite(const std::string path) {
	sprite = WSSprite(gameObject.lock()->GetManager().lock()->GetScene().lock()->GetAssetManager().lock()->GetTexture(path));
}
