// Modified version of https://github.com/MrPlow442/Box2D-SFML2-Debug-Draw/blob/master/Box2D-SFML%20Debug%20Draw/SFMLDebugDraw.cpp

#pragma once
#ifndef WS_DEBUG_DRAW_H
#define WS_DEBUG_DRAW_H
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Physics.h"
namespace sfdd {
	const float SCALE = 32.f;
}

class DebugDraw : public b2Draw {
public:
	DebugDraw();
	static sf::Color GLColorToSFML(const b2Color &color, sf::Uint8 alpha = 255) {
		return sf::Color(static_cast<sf::Uint8>(color.r * 255), static_cast<sf::Uint8>(color.g * 255), static_cast<sf::Uint8>(color.b * 255), alpha);
	}
	static sf::Vector2f B2VecToSFVec(const b2Vec2 &vector, bool scaleToPixels = true) {
		return sf::Vector2f(vector.x * Physics::PIXELS_PER_METRE, vector.y  * Physics::PIXELS_PER_METRE);
	}
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	void DrawTransform(const b2Transform& xf) override;
	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override;
};
#endif // !WS_DEBUG_DRAW_H