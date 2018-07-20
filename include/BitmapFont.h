#pragma once
#ifndef WS_BITMAP_FONT_H
#define WS_BITMAP_FONT_H
#include <memory>
#include <vector>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Vertex.hpp>
#include <SFML\Graphics\RenderStates.hpp>
namespace WeiSungEngine {
	struct BitmapGlyph {
		int charIndex = 0;
		sf::IntRect textureBounds;
		std::vector<sf::Vertex> verts;
	};

	class BitmapFont : public std::enable_shared_from_this<BitmapFont> {
	public:
		BitmapFont();
		~BitmapFont();
		void Load(const std::string & fileName = "FontData");
		const bool IsValid() const;
		void SetGlyph(const unsigned int index, const BitmapGlyph & glyph);
		BitmapGlyph GetGlyph(const unsigned int index);
		void SetSpacing(const unsigned int spacing);
		unsigned int GetSpacing();
		const unsigned int NumberOfCharacters() const;
		friend std::ostream& operator<< (std::ostream& stream, const std::shared_ptr<BitmapFont> & font);
		sf::Texture * GetTexture() const;
		std::string ToString();
	private:
		sf::Texture * texture;
		bool valid = false;
		sf::RenderStates renderState;
		unsigned int width = 0, height = 0, tileWidth = 0, tileHeight = 0, startIndex = 0, spacing = 0;
		std::string name = "";
		std::vector<BitmapGlyph> characters;
	};
}

#endif // !WS_BITMAP_FONT_H
