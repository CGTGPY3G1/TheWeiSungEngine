#pragma once
#ifndef WS_VECTOR2_H
#define WS_VECTOR2_H

#include<iostream>
#include <string>
#include <SFML\System\Vector2.hpp>
class Vector2 {
public:
	operator sf::Vector2f() {
		return sf::Vector2f(x, y);
	}
	operator sf::Vector2u() {
		return sf::Vector2u((unsigned int)x, (unsigned int)y);
	}
	operator sf::Vector2i() {
		return sf::Vector2i((int)x, (int)y);
	}

	const static Vector2 Up, Down, Left, Right, Zero, One;
	float x, y;
	Vector2();
	Vector2(const float & x, const float & y);
	Vector2(const sf::Vector2f & in) {
		x = in.x, y = in.y;
	}
	Vector2(const sf::Vector2i & in) {
		x = (float)in.x, y = (float)in.y;
	}
	Vector2(const sf::Vector2u & in) {
		x = (float)in.x, y = (float)in.y;
	}
	~Vector2();
	Vector2 Set(const float & x, const float & y);
	Vector2 Set(const Vector2 & toSet);
	void operator = (const Vector2 & toSet);
	Vector2 Add(const float & x, const float & y);
	Vector2 operator + (const Vector2 & toAdd);
	const Vector2 operator + (const Vector2 & toAdd) const;
	Vector2 operator += (const Vector2 & toAdd);
	Vector2 Subtract(const float & x, const float & y);
	Vector2 operator - (const Vector2 & toSubtract);
	Vector2 operator - ();
	const Vector2 operator - (const Vector2 & toSubtract) const;
	Vector2 operator -= (const Vector2 & toSubtract);
	Vector2 Scale(const float & scalar);
	Vector2 operator * (const float & scalar);
	const Vector2 operator * (const float & scalar) const;
	Vector2 operator *= (const float & scalar);
	Vector2 operator / (const float & scalar);
	const Vector2 operator / (const float & scalar) const;
	Vector2 operator /= (const float & scalar);
	bool operator == (const Vector2 & other);
	float Dot(const Vector2 & other);
	const float Dot(const Vector2 & other) const;
	float Dot(const float & x, const float & y);
	float Cross(const Vector2 & other);
	float Cross(const float & x, const float & y);
	Vector2 Normalize();
	Vector2 Normalized();
	float SquareMagnitude();
	float Magnitude();
	float AngleInRadians();
	float AngleInDegrees();
	Vector2 RotateInRadians(const float & angle);
	Vector2 RotatedInRadians(const float & angle);
	Vector2 RotateInDegrees(const float & angle);
	Vector2 RotatedInDegrees(const float & angle);
	Vector2 Flip90(const bool & clockwise);
	float AngleToPointInDegrees(const Vector2 & point);
	float AngleToPointInRadians(const Vector2 & point);
	Vector2 RotateToPoint(const Vector2 & point);
	Vector2 RotateToInDegrees(const float & angle);
	Vector2 RotateToInRadians(const float & angle);
	Vector2 Lerp(const Vector2 & target, const float & alpha){
		const float inverse = 1.0f - alpha;
		return Vector2((*this * inverse) + (target * alpha));
	}
	Vector2 Copy();
	const std::string ToString() const {
		return "X = " + std::to_string(x) + " Y = " + std::to_string(y);
	}
	friend std::ostream & operator << (std::ostream & out, const Vector2 & toPrint) {
		return out << "X = " << std::to_string(toPrint.x) << " Y = " << std::to_string(toPrint.y);
	}
	Vector2 & operator = (const sf::Vector2f & in) {
		x = in.x, y = in.y;
		return *this;
	}
	Vector2 & operator = (const sf::Vector2i & in) {
		x = (float)in.x, y = (float)in.y;
		return *this;
	}
	Vector2 & operator = (const sf::Vector2u & in) {
		x = (float)in.x, y = (float)in.y;
		return *this;
	}
	Vector2 & operator += (const sf::Vector2f & in) {
		x += in.x, y += in.y;
		return *this;
	}
	Vector2 & operator += (const sf::Vector2i & in) {
		x += (float)in.x, y += (float)in.y;
		return *this;
	}
	Vector2 & operator += (const sf::Vector2u & in) {
		x += (float)in.x, y += (float)in.y;
		return *this;
	}

	Vector2 & operator - (const sf::Vector2f & in) {
		return Vector2(x - in.x, y - in.y);
	}
	Vector2 & operator - (const sf::Vector2i & in) {
		return Vector2(x - (float)in.x, y - (float)in.y);;
	}
	Vector2 & operator - (const sf::Vector2u & in) {
		return Vector2(x - (float)in.x, y - (float)in.y);
	}

	Vector2 & operator + (const sf::Vector2f & in) {
		return Vector2(x + in.x, y + in.y);
	}
	Vector2 & operator + (const sf::Vector2i & in) {
		return Vector2(x + (float)in.x, y + (float)in.y);
	}
	Vector2 & operator + (const sf::Vector2u & in) {
		return Vector2(x + (float)in.x, y + (float)in.y);
	}

	Vector2 & operator -= (const sf::Vector2f & in) {
		x -= in.x, y -= in.y;
		return *this;
	}
	Vector2 & operator -= (const sf::Vector2i & in) {
		x -= (float)in.x, y -= (float)in.y;
		return *this;
	}
	Vector2 & operator -= (const sf::Vector2u & in) {
		x -= (float)in.x, y -= (float)in.y;
		return *this;
	}


	Vector2 & operator - (const sf::Vector2f & in) const {
		return Vector2(x - in.x, y - in.y);
	}
	Vector2 & operator - (const sf::Vector2i & in) const {
		return Vector2(x - (float)in.x, y - (float)in.y);;
	}
	Vector2 & operator - (const sf::Vector2u & in) const {
		return Vector2(x - (float)in.x, y - (float)in.y);
	}

	Vector2 & operator + (const sf::Vector2f & in) const {
		return Vector2(x + in.x, y + in.y);
	}
	Vector2 & operator + (const sf::Vector2i & in) const {
		return Vector2(x + (float)in.x, y + (float)in.y);
	}
	Vector2 & operator + (const sf::Vector2u & in) const {
		return Vector2(x + (float)in.x, y + (float)in.y);
	}

	template <class Archive>
	void load(Archive & ar) {
		ar(cereal::make_nvp("X", x), cereal::make_nvp("Y", y));
	}

	template <class Archive>
	void save(Archive & ar) const {
		ar(cereal::make_nvp("X", x), cereal::make_nvp("Y", y));
	}
};

#endif // !WS_VECTOR2_H