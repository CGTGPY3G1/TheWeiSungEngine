#include "ParsedTMX.h"
#include "Debug.h"
TmxMap::TmxMap(std::string xml) {
	Load(xml);
}

bool TmxMap::Load(std::string xml) {
	try {
		char * c = (char *)xml.c_str();
		cereal::rapidxml::xml_document<> doc;
		doc.parse<0>(c);
		cereal::rapidxml::xml_node<> *mapnode = doc.first_node("map");
		this->width = atoi(mapnode->first_attribute("width")->value());
		this->height = atoi(mapnode->first_attribute("height")->value());
		this->tileWidth = atoi(mapnode->first_attribute("tilewidth")->value());
		this->tileHeight = atoi(mapnode->first_attribute("tileheight")->value());
		cereal::rapidxml::xml_node<> *tilesetnode = mapnode->first_node("tileset");
		while(tilesetnode) {
			std::shared_ptr<TmxTileset> tileset = std::make_shared<TmxTileset>();
			tileset->firstGid = atoi(tilesetnode->first_attribute("firstgid")->value());
			tileset->name = tilesetnode->first_attribute("name")->value();
			tileset->tileWidth = atoi(tilesetnode->first_attribute("tilewidth")->value());
			tileset->tileHeight = atoi(tilesetnode->first_attribute("tileheight")->value());
			cereal::rapidxml::xml_node<> *imageNode = tilesetnode->first_node("image");
			if(imageNode) {
				cereal::rapidxml::xml_attribute<> * attributeNode = imageNode->first_attribute("source");
				tileset->filename = attributeNode->value();
				attributeNode = attributeNode->next_attribute("width");
				tileset->sourceWidth = atoi(attributeNode->value());
				tileset->sourceHeight = atoi(attributeNode->next_attribute("height")->value());
			}
			this->tilesets.push_back(tileset);
			tilesetnode = tilesetnode->next_sibling("tileset");
		}
		const char *separators = " \t,\n\r";
		cereal::rapidxml::xml_node<> *layernode = mapnode->first_node("layer");
		while(layernode) {
			std::shared_ptr<TmxLayer> layer = std::make_shared<TmxLayer>();
			layer->name = layernode->first_attribute("name")->value();
			layer->width = atoi(layernode->first_attribute("width")->value());
			layer->height = atoi(layernode->first_attribute("height")->value());
			const char * data = layernode->first_node("data")->value();
			layer->data = std::vector<int>(layer->width * layer->height);
			int size = strlen(data) + 1;
			char * copy = (char*)malloc(strlen(data) + 1);
			strcpy_s(copy, size, data);
			char * bufferItem;
			char * item = strtok_s(copy, separators, &bufferItem);
			int index = 0;
			while(item) {
				layer->data[index] = atoi(item);
				index++;
				item = strtok_s(0, separators, &bufferItem);
			}
			free(copy);
			this->layers.push_back(layer);
			layernode = layernode->next_sibling("layer");
		}
		cereal::rapidxml::xml_node<> *objectgroupnode = mapnode->first_node("objectgroup");
		while(objectgroupnode != NULL) {
			std::shared_ptr<TmxGroup> group = std::make_shared<TmxGroup>();
			group->name = objectgroupnode->first_attribute("name")->value();
			cereal::rapidxml::xml_attribute<> *visibleattr = objectgroupnode->first_attribute("visible");
			if(visibleattr) group->visible = atoi(visibleattr->value());
			else group->visible = true;
			cereal::rapidxml::xml_node<> *objectnode = objectgroupnode->first_node("object");
			while(objectnode) {
				std::shared_ptr<TmxObject> object = std::make_shared<TmxObject>();
				auto nameattr = objectnode->first_attribute("name");
				if(nameattr) object->name = nameattr->value();

				auto typeattr = objectnode->first_attribute("type");
				if(typeattr) object->type = atoi(typeattr->value());

				auto gidattr = objectnode->first_attribute("gid");
				if(gidattr) object->gid = atoi(gidattr->value());

				object->x = (float)atof(objectnode->first_attribute("x")->value());
				object->y = (float)atof(objectnode->first_attribute("y")->value());
				auto widthattr = objectnode->first_attribute("width");
				if(widthattr) {
					object->width = (float)atof(widthattr->value());
				}
				auto heightattr = objectnode->first_attribute("height");
				if(heightattr) {
					object->height = (float)atof(heightattr->value());
				}
				auto rotationattr = objectnode->first_attribute("rotation");
				
				if(rotationattr) object->rotation = (float)atof(rotationattr->value());

				cereal::rapidxml::xml_node<> *propertiesnode = objectnode->first_node("properties");
				if(propertiesnode) {
					cereal::rapidxml::xml_node<> *propertynode = propertiesnode->first_node("property");
					while(propertynode) {
						std::shared_ptr<TmxProperty> prop = std::make_shared<TmxProperty>();
						prop->name = propertynode->first_attribute("name")->value();
						prop->value = propertynode->first_attribute("value")->value();
						object->properties.push_back(prop);
						propertynode = propertynode->next_sibling("property");
					}
				}
				group->objects.push_back(object);
				objectnode = objectnode->next_sibling("object");
			}
			this->groups.push_back(group);
			objectgroupnode = objectgroupnode->next_sibling("objectgroup");
		}
		isValid = true;
	}
	catch(const std::exception& e) {
		if(isValid) isValid = false;
		Debug::GetInstance().Log("Error Loading TMX file : " + std::string(e.what()), DebugMessageType::DEBUG_TYPE_FAILURE_MEDIUM);
	}
	return isValid;
}