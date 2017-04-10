#pragma once
#ifndef WS_BITMAP_STRING_H
#define WS_BITMAP_STRING_H
#include <memory>
#include <string>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Color.hpp>
#include <SFML\Graphics\VertexArray.hpp>

class BitmapFont;
class BitmapString : public sf::Drawable, public sf::Transformable {
public:
	BitmapString();
	BitmapString(const std::string &string, const std::shared_ptr<BitmapFont> & font, const sf::Color colour = sf::Color::White);
	const std::string& GetString() const;
	void SetColour(const float & r, const float & g, const float & b, const float & a);
	sf::Color GetColour() const;
	sf::FloatRect GetBounds();
	sf::Vector2f GetSize() const;
	void SetSize(const sf::Vector2f & newSize);
private:
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	void BuildBitmapString();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::shared_ptr<BitmapFont> drawFont;
	sf::VertexArray vertexArray;
	std::string toPrint;
	sf::Color drawColour;
	sf::FloatRect  bounds;
};


#endif // !WS_BITMAP_FONT_H