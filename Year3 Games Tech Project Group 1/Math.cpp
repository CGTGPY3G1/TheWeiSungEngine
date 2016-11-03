#include "Math.h"


float Math::PI() {
	const static float TO_RETURN = 3.1415927f;
	return TO_RETURN;
}

float Math::TAU() {
	const static float TO_RETURN = PI() * 2;
	return TO_RETURN;
}

float Math::RadiansToDegrees() {
	const static float TO_RETURN = 180.0f / PI();
	return TO_RETURN;
}

float Math::DegreesToRadians() {
	const static float TO_RETURN = PI() / 180.0f;
	return TO_RETURN;
}


