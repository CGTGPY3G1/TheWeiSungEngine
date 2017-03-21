#include "BitmapString.h"
#include "BitmapFont.h"
#include "TypeConversion.h"

BitmapString::BitmapString() {
}

BitmapString::BitmapString(const std::string & string, const std::shared_ptr<BitmapFont> & font, const sf::Color colour) : toPrint(string), drawFont(font), vertexArray(sf::Quads), drawColour(colour) {
	unsigned int noOfChars = string.size();
	int nextX = 0;
	for(size_t i = 0; i < noOfChars; i++) {
		BitmapGlyph glyph = font->GetGlyph((int)string[i]);
		for(size_t j = 0; j < glyph.verts.size(); j++) {
			sf::Vertex v = glyph.verts[j];
			v.position.x += nextX;
			vertexArray.append(v);
		}
		nextX += glyph.textureBounds.width;
	}
	bounds = vertexArray.getBounds();
}

const std::string & BitmapString::GetString() const {
	return toPrint;
}

void BitmapString::SetColour(const float & r, const float & g, const float & b, const float & a) {
	drawColour = TypeConversion::ConvertToSFColour(r, g, b, a);
	for(size_t i = 0; i < vertexArray.getVertexCount(); i++) {
		vertexArray[i].color = drawColour;
	}
}

sf::Color BitmapString::GetColour() const {
	return drawColour;
}

sf::FloatRect BitmapString::GetBounds() {
	return sf::FloatRect(0.0f, 0.0f, bounds.width, bounds.height);
}

sf::Vector2f BitmapString::GetSize() const {
	const sf::FloatRect b = getTransform().transformRect(bounds);
	return sf::Vector2f(b.width, b.height);
}

void BitmapString::SetSize(const sf::Vector2f & newSize) {
	sf::Vector2f currentSize = GetSize();
	scale(newSize.x / currentSize.x, newSize.y / currentSize.y);
}

void BitmapString::SetUniformSize(const float & newSize, const bool & x) {
	sf::Vector2f currentSize = GetSize();
	float newScale = x ? newSize / currentSize.x : newSize / currentSize.y;
	scale(newScale, newScale);
}

sf::FloatRect BitmapString::getLocalBounds() const {
	return sf::FloatRect(0.0f, 0.0f, bounds.width, bounds.height);
}

sf::FloatRect BitmapString::getGlobalBounds() const {
	return getTransform().transformRect(getLocalBounds());
}

void BitmapString::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	if(drawFont) {
		states.transform *= getTransform();
		states.texture = drawFont->GetTexture();
		target.draw(vertexArray, states);
	}
}
