#pragma once
#ifndef WS_AABB_H
#include "Vector2.h"
namespace WeiSungEngine {
	struct AABB {
		float top, left, right, bottom;

		AABB(const Vector2 & topLeft, const Vector2 & bottomRight) {
			top = topLeft.y; left = topLeft.x; bottom = bottomRight.y; right = bottomRight.x;
		}

		AABB(const float & top, const float & left, const float & bottom, const float & right) {
			this->top = top; this->left = left; this->bottom = bottom; this->right = right;
		}


	};
}
#endif // !WS_RECTANGLE_H
