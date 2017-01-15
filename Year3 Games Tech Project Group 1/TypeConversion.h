#pragma once
#ifndef WS_SFML_CONVERSION_H
#define WS_SFML_CONVERSION_H
#include <SFML\Graphics.hpp>
#include <Box2D\Common\b2Math.h>
#include "Graphics.h"
#include "Physics.h"
class TypeConversion {
public:
	TypeConversion() {}
	~TypeConversion() {}

	static Vector2 ConvertToVector2(const sf::Vector2f & v) {
		return Vector2(v.x, v.y);
	}

	static Vector2 ConvertToVector2(const sf::Vector2i & v) {
		return Vector2((float)v.x, (float)v.y);
	}

	static Vector2 ConvertToVector2(const sf::Vector2u & v) {
		return Vector2((float)v.x, (float)v.y);
	}

	static Vector2 ConvertToVector2(const b2Vec2 & v) {
		return Vector2((float)v.x * Physics::PIXELS_PER_METRE, (float)v.y * Physics::PIXELS_PER_METRE);
	}

	static b2Vec2 ConvertToB2Vector2(const Vector2 & v) {
		return b2Vec2((float)v.x * Physics::METRES_PER_PIXEL, (float)v.y * Physics::METRES_PER_PIXEL);
	}

	static b2Vec2 ConvertToB2Vector2(const sf::Vector2f & v) {
		return b2Vec2(v.x * Physics::METRES_PER_PIXEL, v.y  * Physics::METRES_PER_PIXEL);
	}

	static b2Vec2 ConvertToB2Vector2(const sf::Vector2u & v) {
		return b2Vec2((float)v.x * Physics::METRES_PER_PIXEL, (float)v.y * Physics::METRES_PER_PIXEL);
	}

	static b2Vec2 ConvertToB2Vector2(const sf::Vector2i & v) {
		return b2Vec2((float)v.x * Physics::METRES_PER_PIXEL, (float)v.y * Physics::METRES_PER_PIXEL);
	}


	static sf::Vector2f ConvertToSFVector2f(const Vector2 & v) {
		return sf::Vector2f(v.x, v.y);
	}

	static sf::Vector2u ConvertToSFVector2u(const Vector2 & v) {
		return sf::Vector2u((unsigned int)v.x, (unsigned int)v.y);
	}

	static sf::Vector2i ConvertToSFVector2i(const Vector2 & v) {
		return sf::Vector2i((int)v.x, (int)v.y);
	}

	static sf::Vector2f ConvertToSFVector2f(const b2Vec2 & v) {
		return sf::Vector2f(v.x * Physics::PIXELS_PER_METRE, v.y  * Physics::PIXELS_PER_METRE);
	}

	static sf::Vector2u ConvertToSFVector2u(const b2Vec2 & v) {
		return sf::Vector2u((unsigned int)(v.x * Physics::PIXELS_PER_METRE), (unsigned int)(v.y * Physics::PIXELS_PER_METRE));
	}

	static sf::Vector2i ConvertToSFVector2i(const b2Vec2 & v) {
		return sf::Vector2i((int)(v.x * Physics::PIXELS_PER_METRE), (int)(v.y * Physics::PIXELS_PER_METRE));
	}

	static sf::ContextSettings ConvertToSFContext(const GraphicsSettings & g) {
		
		return sf::ContextSettings(g.depthBits, g.stencilBits, g.antialiasingLevel, g.majorversion, g.minorVersion);
	}

	static sf::View ConvertToSFView(const GraphicsSettings & g) {
		return sf::View(ConvertToSFVector2f(g.screenPosition), ConvertToSFVector2f(g.resolution));
	}

	static GraphicsSettings convertToGraphicsSettings(const sf::ContextSettings & c, sf::View v) {
		GraphicsSettings g = GraphicsSettings();
		g.antialiasingLevel = c.antialiasingLevel;
		g.depthBits = c.depthBits;
		g.minorVersion = c.minorVersion;
		g.majorversion = c.majorVersion;
		g.screenPosition = ConvertToVector2(v.getCenter());
		g.resolution = ConvertToVector2(v.getSize());
		return g;
	}
private:

};

#endif // !WS_SFML_CONVERSION_H