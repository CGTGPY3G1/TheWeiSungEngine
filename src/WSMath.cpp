#include "WSMath.h"

namespace WeiSungEngine {
	float WSMath::PI() {
		const static float TO_RETURN = 3.1415927f;
		return TO_RETURN;
	}

	float WSMath::TAU() {
		const static float TO_RETURN = PI() * 2;
		return TO_RETURN;
	}

	float WSMath::RadiansToDegrees() {
		const static float TO_RETURN = 180.0f / PI();
		return TO_RETURN;
	}

	float WSMath::DegreesToRadians() {
		const static float TO_RETURN = PI() / 180.0f;
		return TO_RETURN;
	}
}

