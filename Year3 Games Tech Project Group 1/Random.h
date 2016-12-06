#pragma once
#ifndef WS_RANDOM_H
#define WS_RANDOM_H

class Random {
public:
	~Random();
	static int RandomInt(unsigned int max);
	static int RandomInt(unsigned int min, unsigned int max);
	static float RandomFloat(const float & max);
	static float RandomFloat(const float & min, const float & max);
	static bool RandomBool();
private:
	static void Seed();
	static bool initialised;
};

#endif // !WS_RANDOM_H