#pragma once
#ifndef WS_RECT_H
#define WS_RECT_H

struct WSRect {
	WSRect() : x(0), y(0), width(1), height(1) { }
	WSRect(const int & nX, const int & nY, const int & nWidth, const int & nHeight) : x(nX), y(nY), width(nWidth), height(nHeight) {}
	int x, y, width, height;
};

#endif // !WS_RECT_H