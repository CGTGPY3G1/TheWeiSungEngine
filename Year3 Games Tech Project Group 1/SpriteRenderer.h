#pragma once
#ifndef WS_SPRITE_RENDERER_H
#define WS_SPRITE_RENDERER_H
#include"Component.h"
#include "WSSprite.h"
#include "RenderLayer.h"
class SpriteRenderer : public Component {
public:
	SpriteRenderer();
	SpriteRenderer(std::weak_ptr<GameObject> gameObject);
	~SpriteRenderer();
	const ComponentType Type() const override { return COMPONENT_SPRITE_RENDERER; }
	int GetSortOrder();
	void SetSortOrder(const int & newOrder);
	RenderLayer GetSortLayer();
	void SetSortLayer(const RenderLayer & newLayer);
	float GetPixelsPeMetre();
	void SetPixelsPeMetre(const float & PPM);
	WSSprite & GetSprite();
	void Init(const WSSprite & sprite, const RenderLayer & sortLayer = RenderLayer::MIDGROUND_LAYER, const int & sortOrder = 0);
	void Init(const std::string path, const RenderLayer & sortLayer = RenderLayer::MIDGROUND_LAYER, const int & sortOrder = 0);
	void SetSprite(const WSSprite & sprite);
	void LoadSprite(const std::string path);
private:
	WSSprite sprite;
	int sortOrder;
	RenderLayer sortLayer;
	float pixelsPerMeter;
};

#endif // !WS_SPRITE_RENDERER_H