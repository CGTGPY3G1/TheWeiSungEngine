#include "Random.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
namespace WeiSungEngine {
	bool Random::initialised = false;

	Random::~Random() {
	}

	int Random::RandomInt(unsigned int max) {
		if (!initialised) Seed();
		return rand() % max;
	}

	int Random::RandomInt(unsigned int min, unsigned int max) {
		if (!initialised) Seed();
		return min + (rand() % (max - min));
	}

	float Random::RandomFloat(const float & max) {
		if (!initialised) Seed();
		float toReturn = ((float)rand() / RAND_MAX) * max;
		return toReturn;
	}

	float Random::RandomFloat(const float & min, const float & max) {
		if (!initialised) Seed();
		return min + (((float)rand() / RAND_MAX) * std::abs(max - min));
	}

	bool Random::RandomBool() {
		if (!initialised) Seed();
		return (rand() % 2) == 1 ? true : false;
	}

	void Random::Seed() {
		srand(static_cast<unsigned int>(time(0)));
		initialised = true;
	}
}
