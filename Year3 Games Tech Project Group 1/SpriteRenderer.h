#pragma once
#ifndef WS_SPRITE_RENDERER_H
#define WS_SPRITE_RENDERER_H
#include"Component.h"
#include "WSSprite.h"
#include "RenderLayer.h"

enum PivotPoint { TopLeft, Top, TopRight, Left, Centre, Right, BottomLeft, Bottom, BottomRight };

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
	void SetColour(const float & r, const float & g, const float & b, const float & a);
	WSSprite & GetSprite();
	Vector2 GetSize();
	void SetSize(const Vector2 & size);
	void SetSize(const float & width, const float & height);
	float GetWidth();
	float GetHeight();
	PivotPoint GetPivot();
	void SetPivot(PivotPoint pivot);
	void SetPivotManually(const float & x, const float & y);
	void SetPivotManually(const Vector2 & pivotPoint);
	void Init(const WSSprite & sprite, const PivotPoint & pivot = PivotPoint::Centre, const RenderLayer & sortLayer = RenderLayer::MIDGROUND_LAYER, const int & sortOrder = 0);
	void Init(const WSSprite & sprite, const Vector2 & pivot, const RenderLayer & sortLayer = RenderLayer::MIDGROUND_LAYER, const int & sortOrder = 0);
	void Init(const std::string & path, const PivotPoint & pivot = PivotPoint::Centre, const RenderLayer & sortLayer = RenderLayer::MIDGROUND_LAYER, const int & sortOrder = 0);
	
	void Init(const std::string & path, const Vector2 & pivot, const RenderLayer & sortLayer = RenderLayer::MIDGROUND_LAYER, const int & sortOrder = 0);
	void UpdateOrigin();
	//void Init(const WSSprite & sprite, const int & tx, const int & tY, const int & tWidth, const int &  tHeight, const RenderLayer & sortLayer = RenderLayer::MIDGROUND_LAYER, const int & sortOrder = 0);
	//void Init(const std::string path, const int & tx, const int & tY, const int & tWidth, const int &  tHeight, const RenderLayer & sortLayer = RenderLayer::MIDGROUND_LAYER, const int & sortOrder = 0);
	void SetSprite(const WSSprite & sprite);
	void SetTextureRect(const int & x, const int & y, const int & width, const int & height);
	void LoadSprite(const std::string path);
private:
	WSSprite sprite;
	int sortOrder;
	RenderLayer sortLayer;
	float pixelsPerMeter;
	bool pivotDirty = false;
	PivotPoint pivot = PivotPoint::TopLeft;
};

#endif // !WS_SPRITE_RENDERER_H