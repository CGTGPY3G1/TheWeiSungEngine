#include "BitmapFont.h"
#include <fstream>
#include <iostream>
namespace WeiSungEngine {
	BitmapFont::BitmapFont() {
	}

	BitmapFont::~BitmapFont() {
	}

	void BitmapFont::Load(const std::string & fileName) {
		try {
			bool loadError = false;
			std::ifstream fontData("Font/" + fileName + ".wsfd");
			if (fontData.is_open()) {
				while (fontData.good()) {
					std::string toParse;
					getline(fontData, toParse);
					int startIndex = 0, endIndex = 0;
					if (toParse.find("FontData") != std::string::npos) {
						startIndex = toParse.find_first_of('|') + 1;
						toParse = toParse.substr(startIndex, toParse.size() - startIndex);
						endIndex = toParse.find_first_of('|');
						name = toParse.substr(0, endIndex);
						int values[5];
						for (size_t i = 0; i < 5; i++) {
							startIndex = endIndex + 1;
							toParse = toParse.substr(startIndex, toParse.size() - startIndex);
							endIndex = toParse.find_first_of('|');
							values[i] = std::stoi(toParse.substr(0, endIndex));
						}
						width = values[0];
						height = values[1];
						tileWidth = values[2];
						tileHeight = values[3];
						spacing = values[4];
					}
					else if (toParse.find("CharacterData") != std::string::npos) {
						endIndex = toParse.find_first_of('|');
						int values[5];
						for (size_t i = 0; i < 5; i++) {
							startIndex = endIndex + 1;
							toParse = toParse.substr(startIndex, toParse.size() - startIndex);
							endIndex = toParse.find_first_of('|');
							values[i] = std::stoi(toParse.substr(0, endIndex));
						}
						BitmapGlyph glyph;
						sf::IntRect bounds = sf::IntRect(values[1], values[2], values[3], values[4]);
						glyph.textureBounds = bounds;
						glyph.charIndex = values[0];
						glyph.verts.push_back(sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Vector2f((float)bounds.left, (float)bounds.top)));
						glyph.verts.push_back(sf::Vertex(sf::Vector2f(bounds.width, 0.0f), sf::Vector2f((float)(bounds.left + bounds.width), (float)bounds.top)));
						glyph.verts.push_back(sf::Vertex(sf::Vector2f(bounds.width, bounds.height), sf::Vector2f((float)(bounds.left + bounds.width), (float)(bounds.top + bounds.height))));
						glyph.verts.push_back(sf::Vertex(sf::Vector2f(0.0f, bounds.height), sf::Vector2f((float)bounds.left, (float)(bounds.top + bounds.height))));
						SetGlyph(glyph.charIndex, glyph);
					}
				}
				texture = new sf::Texture();
				loadError = !texture->loadFromFile("Font/" + fileName + ".png");
				texture->setSmooth(true);
				texture->generateMipmap();
				valid = !loadError;
			}
			else {
				valid = false;
			}

		}
		catch (const std::exception&) {
			valid = false;
		}
	}

	const bool BitmapFont::IsValid() const {
		return valid;
	}

	void BitmapFont::SetGlyph(const unsigned int index, const BitmapGlyph & glyph) {
		if (index >= characters.size()) characters.resize(index + 1);
		characters[index] = glyph;
	}

	BitmapGlyph BitmapFont::GetGlyph(const unsigned int index) {
		if (index >= characters.size()) characters.resize(index + 1);
		return characters[index];
	}

	void BitmapFont::SetSpacing(const unsigned int spacing) {
		this->spacing = spacing;
	}

	unsigned int BitmapFont::GetSpacing() {
		return spacing;
	}

	const unsigned int BitmapFont::NumberOfCharacters() const {
		return characters.size();
	}

	sf::Texture * BitmapFont::GetTexture() const {
		return texture;
	}

	std::string BitmapFont::ToString() {
		std::string output = "";
		output += "FontData|" + name + "|" + std::to_string(width) + "|" + std::to_string(height) + "|" + std::to_string(tileWidth) + "|" + std::to_string(tileHeight) + "|" + std::to_string(startIndex) + "\n";
		for (size_t i = 0; i < NumberOfCharacters(); i++) {
			BitmapGlyph glyph = characters[i];
			output += "CharacterData|" + std::to_string(glyph.charIndex) + "|" + std::to_string(glyph.textureBounds.left) + "|" + std::to_string(glyph.textureBounds.top) + "|" + std::to_string(glyph.textureBounds.width) + "|" + std::to_string(glyph.textureBounds.height) + "\n";
		}
		return output;
	}

	std::ostream & operator<<(std::ostream & stream, const std::shared_ptr<BitmapFont> & font) {
		return stream << font->ToString();
	}
}
