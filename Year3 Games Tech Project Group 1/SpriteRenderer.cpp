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

void SpriteRenderer::SetColour(const float & r, const float & g, const float & b, const float & a) {
	sprite.setColor(sf::Color(r * 255, g * 255, b * 255, a * 255));
}

WSSprite & SpriteRenderer::GetSprite() {
	return sprite;
}

Vector2 SpriteRenderer::GetSize() {
	Vector2 scale = GetComponent<Transform2D>().lock()->GetScale();
	sf::FloatRect bounds = sprite.getGlobalBounds();
	return Vector2(bounds.width * scale.x, bounds.height * scale.y);
}

void SpriteRenderer::SetSize(const Vector2 & size) {
	SetSize(size.x, size.y);
}

void SpriteRenderer::SetSize(const float & width, const float & height) {
	Vector2 oldSize = GetSize();
	sprite.setScale(width / oldSize.x, height / oldSize.y);
}

float SpriteRenderer::GetWidth() {
	Vector2 scale = GetComponent<Transform2D>().lock()->GetScale();
	sf::FloatRect bounds = sprite.getGlobalBounds();
	return bounds.width * scale.x;
}

float SpriteRenderer::GetHeight() {
	Vector2 scale = GetComponent<Transform2D>().lock()->GetScale();
	sf::FloatRect bounds = sprite.getGlobalBounds();
	return bounds.height * scale.y;
}

PivotPoint SpriteRenderer::GetPivot() {
	return pivot;
}

void SpriteRenderer::SetPivot(PivotPoint pivot) {
		this->pivot = pivot;
		sf::FloatRect bounds = sprite.getGlobalBounds();
		switch(pivot) {
		case TopLeft:
			sprite.setOrigin(0.0f, 0.0f);
			break;
		case Top:
			sprite.setOrigin(bounds.width / 2, 0.0f);
			break;
		case TopRight:
			sprite.setOrigin(bounds.width, 0.0f);
			break;
		case Left:
			sprite.setOrigin(0.0f, bounds.height / 2);
			break;
		case Centre:
			sprite.setOrigin(bounds.width / 2, bounds.height / 2);
			break;
		case Right:
			sprite.setOrigin(bounds.width, bounds.height / 2);
			break;
		case BottomLeft:
			sprite.setOrigin(0.0f, bounds.height);
			break;
		case Bottom:
			sprite.setOrigin(bounds.width / 2, bounds.height);
			break;
		case BottomRight:
			sprite.setOrigin(bounds.width, bounds.height);
			break;
		default:
			break;
		}
}

void SpriteRenderer::Init(const WSSprite & sprite, const PivotPoint & pivot, const RenderLayer & sortLayer, const int & sortOrder) {
	SetSprite(sprite); SetSortLayer(sortLayer); SetSortOrder(sortOrder); SetEnabled(true); SetPivot(pivot);
}

void SpriteRenderer::Init(const std::string & path, const PivotPoint & pivot, const RenderLayer & sortLayer, const int & sortOrder) {
	LoadSprite(path); SetSortLayer(sortLayer); SetSortOrder(sortOrder); SetEnabled(true); SetPivot(pivot);
}

void SpriteRenderer::UpdateOrigin() {
	SetPivot(pivot);
}

void SpriteRenderer::SetSprite(const WSSprite & sprite) {
	this->sprite = sprite;
}

void SpriteRenderer::SetTextureRect(const int & x, const int & y, const int & width, const int & height) {
	sprite.setTextureRect(sf::IntRect(x, y, width, height));
	UpdateOrigin();
}

void SpriteRenderer::LoadSprite(const std::string path) {
	sprite = WSSprite(gameObject.lock()->GetManager().lock()->GetScene().lock()->GetAssetManager().lock()->GetTexture(path));
}
