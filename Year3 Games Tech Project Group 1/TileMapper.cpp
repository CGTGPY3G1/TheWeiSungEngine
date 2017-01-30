#include "TileMapper.h"
#include <queue>
#include <map>
#include "AssetManager.h"
#include "Engine.h"
#include "FileManager.h"
#include "GameObjectFactory.h"
#include <string>
#include "PlayerScript.h"

TileMapper::TileMapper() : Component() {

}

TileMapper::TileMapper(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {

}

TileMapper::~TileMapper() {
}

void TileMapper::Init(const std::string & fileName, const std::string & tilesetName) {
	std::string tmx = FileManager::LoadTMX(fileName);
	LoadTmxMap(tmx, tilesetName);
}

std::vector<std::pair<int, int>> TileMapper::GetPath(NavInfo * start, NavInfo * destination) {
	std::priority_queue<NavInfo *, std::vector<NavInfo *>, std::greater<NavInfo *> > frontier;
	frontier.push(start);
	std::vector<std::pair<int, int>> cameFrom;
	std::unordered_map<NavInfo *, int> currentCost;

	while(!frontier.empty()) {
		NavInfo* current = frontier.top();
		frontier.pop();
		if(current == destination) break;
		for(std::vector<NavInfo *>::iterator i = current->neighbours.begin(); i != current->neighbours.end(); i++) {
			NavInfo * next = (*i);
			int newCost = currentCost[current] + (*i)->cost;

			if(!currentCost.count(next) || newCost < currentCost[next]) {
				currentCost[next] = newCost;
				next->priority = newCost + TileMapper::GetHeuristic(next, destination);
				frontier.push(next);
				cameFrom.push_back(std::pair<int, int>(current->x, current->y));
			}
		}
	}

	return cameFrom;
}

int TileMapper::GetHeuristic(NavInfo * current, NavInfo * destination) {
	int x1 = current->x, y1 = current->y, x2 = destination->x, y2 = destination->y;
	return abs(x1 - x2) + abs(y1 - y2);
}

bool TileMapper::LoadTmxMap(const std::string & xml, const std::string & tilesetName) {

	map = std::make_shared<TmxMap>(xml);
	
	if(map->isValid) {
		bool foundSet = false;
		std::shared_ptr<TmxTileset> tileset;
		const unsigned int noOfTilesets = map->tilesets.size();
		for(size_t i = 0; i < noOfTilesets && !foundSet; i++) {
			if(tilesetName == map->tilesets[i]->name) {
				tileset = map->tilesets[i];
				foundSet = true;
			}
		}
		sf::Texture & texture = AssetManager::GetInstance().GetTexture("Tilesets/" + tileset->filename);
		const unsigned int noOfLayers = map->layers.size();
		for(size_t j = 0; j < noOfLayers; j++) {
			std::shared_ptr<TmxLayer> layer = map->layers[j];
			layer->name;
			if(layer->name == "BackgroundLayer") ProcessTmxTileLayer(tileset, layer, texture);			
		}
		const unsigned int noOfGroups = map->groups.size();
		for(size_t k = 0; k < noOfGroups; k++) {
			std::shared_ptr<TmxGroup> group = map->groups[k];
			group->name;
			if(group->name == "Buildings") ProcessTmxBuildingGroup(group);
			else if(group->name == "Characters") ProcessCharacters(group);
			else if(group->name == "Vehicles") ProcessVehicles(group);
			
		}
	}
	return map->isValid;
}



void TileMapper::ProcessTmxBuildingGroup(std::shared_ptr<TmxGroup> group) {
	std::shared_ptr<Transform2D> transform = GetComponent<Transform2D>().lock();
	Vector2 scale = transform->GetScale();
	const float halfWidth = (float)((map->width - 1 )* map->tileWidth) / 2;
	const float halfHeight = (float)(map->height * map->tileHeight) / 2;
	const unsigned int noOfObjects = group->objects.size();
	
	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<TmxObject> object = group->objects[i];
		
		const unsigned int noOfProperties = object->properties.size();
		const Vector2 objectScale = GetBuildingScale(object->type, object->width, object->height);
		const Vector2 adjustedScale = Vector2(scale.x * objectScale.x, scale.y * objectScale.y);
		Vector2 position = Vector2((object->x - halfWidth) * scale.x, (object->y - halfHeight) * scale.y) + Vector2((object->width / 2) * scale.x, (object->height / 2) * -scale.y).RotateInDegrees(object->rotation);
		std::shared_ptr<GameObject> newBuilding = GameObjectFactory::CreateBuilding(object->type, position, adjustedScale, object->rotation).lock();
	}
}

void TileMapper::ProcessTmxTileLayer(std::shared_ptr<TmxTileset> tileset, std::shared_ptr<TmxLayer> layer, sf::Texture & texture) {
	const float halfWidth = (float)((map->width - 1) * map->tileWidth) / 2;
	const float halfHeight = (float)(map->height * map->tileHeight) / 2;
	std::vector<int> data = layer->data;
	const unsigned int noOfTiles = data.size();
	int sourceColumns = tileset->sourceWidth / tileset->tileWidth;
	for(size_t i = 0; i < noOfTiles; i++) {
		int x = (int)std::roundl((layer->data[i] - 1) % sourceColumns);
		int y = (int)std::roundl((layer->data[i] - 1) / sourceColumns);
		int ax = (i % layer->width), ay = (i / layer->width);

		if(ax == 0) tiles.push_back(std::vector<Tile>());

		sf::IntRect rect = sf::IntRect((x*(tileset->tileWidth+2) + 1), (y*(tileset->tileHeight+2) + 1), tileset->tileWidth, tileset->tileHeight);
		sf::Sprite sprite(texture, rect);
		sf::Vector2f position = sf::Vector2f(std::round(-halfWidth + (ax * tileset->tileWidth)), std::round(-halfHeight + (ay * tileset->tileWidth)));
		sprite.setPosition(std::round(-halfWidth + (ax * tileset->tileWidth)), std::round(-halfHeight + (ay * tileset->tileWidth)));
		
		Tile toAdd;
		toAdd.verts.push_back(sf::Vertex(position, sf::Vector2f((float)rect.left, (float)rect.top)));
		toAdd.verts.push_back(sf::Vertex(sf::Vector2f(position.x + tileset->tileWidth, position.y), sf::Vector2f((float)(rect.left + rect.width), (float)rect.top)));
		toAdd.verts.push_back(sf::Vertex(sf::Vector2f(position.x + tileset->tileWidth, position.y + tileset->tileHeight), sf::Vector2f((float)(rect.left + rect.width), (float)(rect.top + rect.height))));
		toAdd.verts.push_back(sf::Vertex(sf::Vector2f(position.x, position.y + tileset->tileHeight), sf::Vector2f((float)rect.left, (float)(rect.top + rect.height))));
		toAdd.sprite = sprite;
		toAdd.id = layer->data[i];
		tiles[ay].push_back(toAdd);
	}
}

void TileMapper::ProcessCharacters(std::shared_ptr<TmxGroup> group) {
	std::shared_ptr<Transform2D> transform = GetComponent<Transform2D>().lock();
	Vector2 scale = transform->GetScale();
	const float halfWidth = (float)((map->width - 1)* map->tileWidth) / 2;
	const float halfHeight = (float)(map->height * map->tileHeight) / 2;
	const unsigned int noOfObjects = group->objects.size();

	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<TmxObject> object = group->objects[i];

		const unsigned int noOfProperties = object->properties.size();
		Vector2 position = Vector2((object->x - halfWidth)* scale.x, (object->y - halfHeight) * scale.y) + Vector2((object->width / 2) * scale.x, (object->height / 2) * -scale.y).RotateInDegrees(object->rotation);
		if(object->name == "Player") {
			std::shared_ptr<GameObject> player = GameObjectFactory::CreateCharacter(object->name, object->type, position, Vector2::One, object->rotation).lock();
			std::shared_ptr<CircleCollider> playerSensor = player->AddComponent<CircleCollider>().lock();
			playerSensor->Init(Vector2(), 200.0f, true);
			std::shared_ptr<PlayerScript> playerScript = player->AddComponent<PlayerScript>().lock();
			playerScript->Start();
		}
		else {
			GameObjectFactory::CreateCharacter(object->name, object->type, position, Vector2::One, object->rotation).lock();
		}
	}
}

void TileMapper::ProcessVehicles(std::shared_ptr<TmxGroup> group) {
	std::shared_ptr<Transform2D> transform = GetComponent<Transform2D>().lock();
	Vector2 scale = transform->GetScale();
	const float halfWidth = (float)((map->width - 1)* map->tileWidth) / 2;
	const float halfHeight = (float)(map->height * map->tileHeight) / 2;
	const unsigned int noOfObjects = group->objects.size();

	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<TmxObject> object = group->objects[i];

		const unsigned int noOfProperties = object->properties.size();
		Vector2 position = Vector2((object->x - halfWidth)* scale.x, (object->y - halfHeight) * scale.y) + Vector2((object->width / 2) * scale.x, (object->height / 2) * -scale.y).RotateInDegrees(object->rotation);
		GameObjectFactory::CreateVehicle(object->type, position, Vector2::One, object->rotation).lock();
	}
}


bool TileMapper::IsValid() {
	if(!map) return false;
	return map->isValid;
}

void TileMapper::Draw() {
	if(map) {
		std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
		if(!myTransform.lock()) {
			myTransform = GetComponent<Transform2D>();
		}
		std::shared_ptr<Transform2D> transform = myTransform.lock();
		if(transform) {
			Vector2 goPosition = transform->GetPosition();
			Vector2 scale = transform->GetScale();
			sf::Vector2f size = graphics->GetView().getSize(), position = graphics->GetView().getCenter();
			const unsigned int halfWidth = map->width / 2, halfHeight = map->height / 2;
			const int top = (int)(((position.y - goPosition.y - (size.y * 0.5f)) / (scale.y * map->tileHeight)) + halfHeight),
				left = (int)(((position.x - goPosition.x - (size.x * 0.5f)) / (scale.x * map->tileWidth)) + halfWidth - 1),
				bottom = (int)(((position.y - goPosition.y + (size.y * 0.5f)) / (scale.y * map->tileHeight)) + halfHeight + 1),
				right = (int)(((position.x - goPosition.y + (size.x * 0.5f)) / (scale.x * map->tileWidth)) + halfWidth);
			int drawCalls = 0;

			const unsigned int tilesHigh = tiles.size(), tilesWide = tiles[0].size();
			const unsigned int minY = top < 0 ? 0 : top;
			const unsigned int maxY = ((bottom < tilesHigh) ? bottom : (tilesHigh - 1));
			const unsigned int minX = left < 0 ? 0 : left;
			const unsigned int maxX = ((right < tilesWide) ? right : (tilesWide - 1));
			const unsigned int drawWidth = (maxX - minX), drawHeight = (maxY - minY);
			sf::VertexArray vertexArray(sf::Quads, 0);

			vertexArray.setPrimitiveType(sf::Quads);
			unsigned int arraySize = drawWidth * drawHeight * 4;
			if(arraySize > 0 && arraySize < 10000000) {
				int spriteCount = 0, vertCount = 300;
				vertexArray.resize(arraySize);
				int xCount = 0, yCount = 0;
				sf::RenderStates rs;
				rs.transform = transform->GetWorldTransform();
				bool first = true;

				for(unsigned int i = minY; i <= maxY; i++, yCount++) {
					for(unsigned int j = minX; j <= ((unsigned int)right >= tiles[i].size() ? tiles[i].size() - 1 : right); j++, xCount++) {
						Tile tile = tiles[i][j];
						// Don't use vertex array
						//graphics->Draw(tile.sprite, rs);

						// Use Vertex array
						if(first) {
							rs.texture = tile.sprite.getTexture();
							first = false;
						}
						for(size_t i = 0; i < tile.verts.size(); i++) {
							vertexArray.append(tile.verts[i]);
						}
						/*const sf::Vector2f position = tile.sprite.getPosition();
						const sf::FloatRect f = tile.sprite.getGlobalBounds();
						const sf::IntRect texRect = tile.sprite.getTextureRect();
						const float h = f.height, w = f.width;
						const float offset = 0.375;
						vertexArray.append(sf::Vertex(sf::Vector2f(position.x, position.y), sf::Vector2f(texRect.left + offset, texRect.top + offset)));
						vertexArray.append(sf::Vertex(sf::Vector2f(position.x, position.y + h), sf::Vector2f(texRect.left + offset, texRect.top + texRect.height - offset)));
						vertexArray.append(sf::Vertex(sf::Vector2f(position.x + w, position.y + h), sf::Vector2f(texRect.left + texRect.width - offset, texRect.top + texRect.height - offset)));
						vertexArray.append(sf::Vertex(sf::Vector2f(position.x + w, position.y), sf::Vector2f(texRect.left + texRect.width, texRect.top - offset)));*/
					}
				}
				graphics->Draw(vertexArray, rs);
			}
			
		}
	}
}

Vector2 TileMapper::GetBuildingScale(const unsigned int & buildingType, const float & width, const float & height) {
	switch(buildingType) {
	case 1:
		return Vector2(width / 64.0f, height / 64.0f);
	case 2:
		return Vector2(width / 192.0f, height / 160.0f);
	case 3:
		return Vector2(width / 192.0f, height / 224.0f);
	case 4:
		return Vector2(width / 128.0f, height / 96.0f);
	case 5:
		return Vector2(width / 224.0f, height / 288.0f);
	case 6:
		return Vector2(width / 128.0f, height / 224.0f);
	case 7:
		return Vector2(width / 256.0f, height / 128.0f);
	case 8:
		return Vector2(width / 160.0f, height / 96.0f);
	case 9:
		return Vector2(width / 256.0f, height / 96.0f);
	case 10:
		return Vector2(width / 96.0f, height / 64.0f);
	case 11:
		return Vector2(width / 96.0f, height / 96.0f);
	case 12:
	case 13:
	case 14:
	case 15:
		return Vector2(width / 160.0f, height / 64.0f);
	case 16:
	case 17:
	case 18:
		return Vector2(width / 128.0f, height / 352.0f);
	case 19:
	case 20:
		return Vector2(width / 192.0f, height / 128.0f);
	default:
		break;
	}
	return Vector2::One;
}
