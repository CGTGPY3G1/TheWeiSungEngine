#include "Vector2.h"
#include "Math.h"
#include <cmath>

const Vector2 Vector2::Up = Vector2(0.0, 1.0), Vector2::Down = Vector2(0.0, -1.0), Vector2::Left = Vector2(-1.0, 0.0), Vector2::Right = Vector2(1.0, 0.0), Vector2::Zero = Vector2(0.0, 0.0), Vector2::One = Vector2(1.0, 1.0);

Vector2::Vector2() {
	x = 0.0; this->y = 0.0;
}

Vector2::Vector2(const float & x, const float & y) {
	this->x = x; this->y = y;
}

Vector2::~Vector2() {
}

Vector2 Vector2::Set(const float & x, const float & y) {
	this->x = x; this->y = y;
	return *this;
}

Vector2 Vector2::Set(const Vector2 & toSet) {
	x = toSet.x; y = toSet.y;
	return *this;
}

Vector2 Vector2::operator = (const Vector2 & toSet) {
	x = toSet.x; y = toSet.y;
	return *this;
}

Vector2 Vector2::Add(const float & x, const float & y) {
	this->x += x; this->y += y;
	return *this;
}

Vector2 Vector2::operator + (const Vector2 & toAdd) {
	return Vector2(x + toAdd.x, y + toAdd.y);
}

const Vector2 Vector2::operator+(const Vector2 & toAdd) const {
	return Vector2(x + toAdd.x, y + toAdd.y);
}

Vector2 Vector2::operator += (const Vector2 & toAdd) {
	return Add(toAdd.x, toAdd.y);
}

Vector2 Vector2::Subtract(const float & x, const float & y) {
	this->x -= x; this->y -= y;
	return *this;
}

Vector2 Vector2::operator - (const Vector2 & toSubtract) {
	return Vector2(x - toSubtract.x, y - toSubtract.y);
}

Vector2 Vector2::operator - () {
	return Vector2(-x, -y);
}

const Vector2 Vector2::operator - (const Vector2 & toSubtract) const {
	return Vector2(x - toSubtract.x, y - toSubtract.y);
}

Vector2 Vector2::operator -= (const Vector2 & toSubtract) {
	return Subtract(toSubtract.x, toSubtract.y);
}

Vector2 Vector2::Scale(const float & scalar) {
	this->x *= x; this->y *= y;
	return *this;
}

Vector2 Vector2::operator * (const float & scalar) {
	return Vector2(x * scalar, y * scalar);
}

const Vector2 Vector2::operator*(const float & scalar) const {
	return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator *= (const float & scalar) {
	return Scale(scalar);
}

Vector2 Vector2::operator / (const float & scalar) {
	return Vector2(x / scalar, y / scalar);
}

const Vector2 Vector2::operator/(const float & scalar) const {
	return Vector2(x / scalar, y / scalar);
}

Vector2 Vector2::operator /= (const float & scalar) {
	x /= scalar; y /= scalar;
	return *this;
}

bool Vector2::operator==(const Vector2 & other) {
	return x == other.x && y == other.y;
}

float Vector2::Dot(const Vector2 & other) {
	return Dot(other.x, other.y);
}

float Vector2::Dot(const float & x, const float & y) {
	return (this->x * x) + (this->y * y);
}

float Vector2::Cross(const Vector2 & other) {
	return Cross(other.x, other.y);
}

float Vector2::Cross(const float & x, const float & y) {
	return this->x * y - this->y * x;
}

float Vector2::SquareMagnitude() {
	return x * x + y * y;
}

float Vector2::Magnitude() {
	float sqr = SquareMagnitude();
	return (sqr == 0.0 || sqr == 1.0) ? sqr : sqrt(sqr);
}

Vector2 Vector2::Normalize() {
	float mag = Magnitude();
	if(mag != 0.0 && mag != 1.0) {
		x /= mag;
		y /= mag;
	}
	return *this;
}

Vector2 Vector2::Normalized() {
	return Vector2(x, y).Normalize();
}

float Vector2::AngleInRadians() {
	return atan2(y, x);
}

float Vector2::AngleInDegrees() {
	return AngleInRadians() * Math::RadiansToDegrees();
}

Vector2 Vector2::RotateInRadians(const float & angle) {
	float cosine = cos(angle), sine = sin(angle);
	float rotatedX = (x * cosine) - (y * sine), rotatedY = (x * sine) + (y * cosine);
	x = rotatedX; y = rotatedY;
	return *this;
}

Vector2 Vector2::RotatedInRadians(const float & angle) {
	return Vector2(x, y).RotateInRadians(angle);
}

Vector2 Vector2::RotateInDegrees(const float & angle) {
	return RotateInRadians(angle * Math::DegreesToRadians());
}

Vector2 Vector2::RotatedInDegrees(const float & angle) {
	return RotatedInRadians(angle * Math::DegreesToRadians());
}

Vector2 Vector2::Flip90(const bool & clockwise) {
	float oldX = x;
	if(clockwise) { x = y; y = -oldX; }
	else { x = -y; y = oldX; }
	return *this;
}

float Vector2::AngleToPointInDegrees(const Vector2 & point) {
	return AngleToPointInRadians(point) * Math::RadiansToDegrees();
}

float Vector2::AngleToPointInRadians(const Vector2 & point) {
	return atan2f(Cross(point), Dot(point));
}

Vector2 Vector2::RotateToPoint(const Vector2 & point) {
	return RotateInRadians(AngleToPointInRadians(point));
}

Vector2 Vector2::RotateToInDegrees(const float & angle) {
	return RotateToInRadians(angle * Math::DegreesToRadians());
}

Vector2 Vector2::RotateToInRadians(const float & angle) {
	x = Magnitude(); y = 0;
	return RotateInRadians(angle);
}

Vector2 Vector2::Copy() {
	return Vector2(x, y);
}