#include "NameGenerator.h"
#include "Random.h"
#include <fstream>
#include <iostream>
namespace WeiSungEngine {
	NameGenerator & NameGenerator::GetInstance() {
		static NameGenerator instance;
		return instance;
	}

	NameGenerator::NameGenerator() {
		std::ifstream nameList("CharacterInfo/FirstNames.txt");
		if (nameList.is_open()) {
			while (nameList.good()) {
				std::string name;
				getline(nameList, name);
				firstNames.push_back(name);
			}
			nameList.close();
		}

		nameList = std::ifstream("CharacterInfo/Surnames.txt");
		if (nameList.is_open()) {
			while (nameList.good()) {
				std::string name;
				getline(nameList, name);
				lastNames.push_back(name);
			}
			nameList.close();
		}
	}

	NameGenerator::~NameGenerator() {
	}

	std::string NameGenerator::GetRandomName() {
		unsigned int index1 = Random::RandomInt(firstNames.size()), index2 = Random::RandomInt(lastNames.size());
		return firstNames[index1] + " " + lastNames[index2];
	}
}
