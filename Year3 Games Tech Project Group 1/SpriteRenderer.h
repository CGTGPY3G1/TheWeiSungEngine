#pragma once
#ifndef WS_SPRITE_RENDERER_H
#define WS_SPRITE_RENDERER_H
#include"Component.h"
#include <SFML\Graphics\Sprite.hpp>
class SpriteRenderer : public Component {
public:
	SpriteRenderer();
	SpriteRenderer(std::weak_ptr<GameObject> gameObject);
	~SpriteRenderer();
	const ComponentType Type() const override { return COMPONENT_SPRITE_RENDERER; }
	int GetSortOrder();
	void SetSortOrder(const int & newOrder);
	int GetSortLayer();
	void SetSortLrder(const int & newLayer);
	float GetPixelsPeMetre();
	void SetPixelsPeMetr(const float & PPM);
	sf::Sprite & GetSprite();
	void SetSprite(const sf::Sprite & sprite);
	void LoadSprite(const std::string path);
private:
	sf::Sprite sprite;
	int sortOrder, sortLayer;
	float pixelsPerMeter;
};

#endif // !WS_SPRITE_RENDERER_H