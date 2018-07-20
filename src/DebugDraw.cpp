// Modified version of https://github.com/MrPlow442/Box2D-SFML2-Debug-Draw/blob/master/Box2D-SFML%20Debug%20Draw/SFMLDebugDraw.cpp

#include "DebugDraw.h"
#include "Engine.h"
#include "Graphics.h"
#include "TypeConversion.h"
namespace WeiSungEngine {
	DebugDraw::DebugDraw() {
		SetFlags(DebugDraw::e_shapeBit);
	}

	void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
		sf::ConvexShape polygon(vertexCount);
		sf::Vector2f center;
		for (int i = 0; i < vertexCount; i++) {
			sf::Vector2f transformedVec = TypeConversion::ConvertToSFVector2f(vertices[i]);
			polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y)));
		}
		polygon.setOutlineThickness(2.f);
		sf::Color fillColour = DebugDraw::GLColorToSFML(color);
		fillColour.a = 100;
		polygon.setFillColor(fillColour);
		polygon.setOutlineColor(DebugDraw::GLColorToSFML(color));
		Engine::GetInstance().GetGraphics().lock()->Draw(polygon);
	}
	void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
		DrawPolygon(vertices, vertexCount, color);
	}
	void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
		sf::CircleShape circle(radius * Physics::PIXELS_PER_METRE);
		circle.setOrigin(radius * Physics::PIXELS_PER_METRE, radius * Physics::PIXELS_PER_METRE);
		circle.setPosition(TypeConversion::ConvertToSFVector2f(center));
		sf::Color fillColour = DebugDraw::GLColorToSFML(color);
		fillColour.a = 100;
		circle.setFillColor(fillColour);
		circle.setOutlineThickness(2.f);
		circle.setOutlineColor(DebugDraw::GLColorToSFML(color));
		Engine::GetInstance().GetGraphics().lock()->Draw(circle);
	}
	void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
		sf::CircleShape circle(radius * Physics::PIXELS_PER_METRE);
		circle.setOrigin(radius * Physics::PIXELS_PER_METRE, radius * Physics::PIXELS_PER_METRE);
		circle.setPosition(TypeConversion::ConvertToSFVector2f(center));
		sf::Color fillColour = DebugDraw::GLColorToSFML(color);
		fillColour.a = 100;
		circle.setFillColor(fillColour);
		circle.setOutlineThickness(2.f);
		circle.setOutlineColor(DebugDraw::GLColorToSFML(color));

		b2Vec2 endPoint = center + radius * axis;
		sf::Vertex line[2] = {
			sf::Vertex(TypeConversion::ConvertToSFVector2f(center), DebugDraw::GLColorToSFML(color)),
			sf::Vertex(TypeConversion::ConvertToSFVector2f(endPoint), DebugDraw::GLColorToSFML(color)),
		};
		Engine::GetInstance().GetGraphics().lock()->Draw(circle);
		Engine::GetInstance().GetGraphics().lock()->DrawLine(line);
	}
	void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
		sf::Vertex line[] = {
			sf::Vertex(TypeConversion::ConvertToSFVector2f(p1), DebugDraw::GLColorToSFML(color)),
			sf::Vertex(TypeConversion::ConvertToSFVector2f(p2), DebugDraw::GLColorToSFML(color))
		};
		Engine::GetInstance().GetGraphics().lock()->DrawLine(line);
	}
	void DebugDraw::DrawTransform(const b2Transform& xf) {
		float lineLength = 0.4f;
		b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
		sf::Vertex redLine[] = {
			sf::Vertex(TypeConversion::ConvertToSFVector2f(xf.p), sf::Color::Red),
			sf::Vertex(TypeConversion::ConvertToSFVector2f(xAxis), sf::Color::Red)
		};
		b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();
		sf::Vertex greenLine[] = {
			sf::Vertex(TypeConversion::ConvertToSFVector2f(xf.p), sf::Color::Green),
			sf::Vertex(TypeConversion::ConvertToSFVector2f(yAxis), sf::Color::Green)
		};
		Engine::GetInstance().GetGraphics().lock()->DrawLine(redLine);
		Engine::GetInstance().GetGraphics().lock()->DrawLine(greenLine);
	}

	void DebugDraw::DrawPoint(const b2Vec2 & p, float32 size, const b2Color & color) {
		sf::Color colour = DebugDraw::GLColorToSFML(color);
		Engine::GetInstance().GetGraphics().lock()->DrawRect(TypeConversion::ConvertToVector2(p), Vector2(size, size), 0, true, colour.r, colour.b, colour.g, colour.a);
	}

}
