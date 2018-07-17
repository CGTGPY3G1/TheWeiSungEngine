#pragma once
#ifndef WS_SPRITE_RENDERER_H
#define WS_SPRITE_RENDERER_H
#include"Component.h"
#include "WSSprite.h"
#include "RenderLayer.h"
#include "PivotPoint.h"
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
	void Init(const std::string & path, const PivotPoint & pivot = PivotPoint::Centre, const RenderLayer & sortLayer = RenderLayer::MIDGROUND_LAYER, const int & sortOrder = 0, const bool & generateMipMaps = true, const bool & setSmooth = true, const bool & setSRGB = false);	
	void Init(const std::string & path, const Vector2 & pivot, const RenderLayer & sortLayer = RenderLayer::MIDGROUND_LAYER, const int & sortOrder = 0, const bool & generateMipMaps = true, const bool & setSmooth = true, const bool & setSRGB = false);
	void UpdateOrigin();
	void SetSprite(const WSSprite & sprite);
	void SetTextureRect(const int & x, const int & y, const int & width, const int & height);
	void LoadSprite(const std::string path, const bool & generateMipMaps = true, const bool & setSmooth = true, const bool & setSRGB = false);
	sf::Shader * GetShader();
	void SetShader(const std::string & frag, const std::string & vert);
	void SetShader(std::shared_ptr<sf::Shader> shader);
	const std::string GetName() const override { return "SpriteRenderer"; }

private:
	WSSprite sprite;
	std::shared_ptr<sf::Shader> shader;
	int sortOrder;
	RenderLayer sortLayer;
	PivotPoint pivot = PivotPoint::TopLeft;
	std::string path, fragName, vertName;
};

#endif // !WS_SPRITE_RENDERER_H