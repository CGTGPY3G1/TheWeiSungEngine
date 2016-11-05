#pragma once
#ifndef WS_TRANSFORM_2D_H
#define WS_TRANSFORM_2D_H
#include <memory>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Transform.hpp>
#include <vector>
#include"Component.h"
#include "Message.h"
#include "Vector2.h"
class Transform2D : public Component, private sf::Transformable {
public:
	Transform2D();
	Transform2D(std::weak_ptr<GameObject> gameObject);
	~Transform2D();
	const ComponentType Type() const override { return COMPONENT_TRANSFORM_2D; }
	Vector2 GetPosition();
	void SetPosition(const Vector2 & newPosition);
	float GetRotation();
	void SetRotation(const float & newRotation);
	Vector2 GetScale();
	void SetScale(const Vector2 & newScale);
	Vector2 GetForward();
	Vector2 GetRight();
	void Move(Vector2 toMove);
	void Translate(const Vector2 & translation);
	void Rotate(const float & angle);
	const sf::Transform & GetLocalTransform();
	const sf::Transform & GetWorldTransform();
	int GetChildCount();
	std::weak_ptr<Transform2D> GetChild(const unsigned int & index);
	void AddChild(std::weak_ptr<Transform2D> child);
	std::weak_ptr<Transform2D> GetParent();
	void SetParent(std::weak_ptr<Transform2D> newParent);
	/*void HandleMessage(const Message<Vector2> & message) override;
	void HandleMessage(const Message<float> & message) override;*/
protected:
	bool dirty = true;
	void CalculateWorldTransform();
	void SetDirty();
	sf::Transformable world;
	std::weak_ptr<Transform2D> parent;
	std::vector<std::weak_ptr<Transform2D>> children;
};

#endif // !WS_TRANSFORM_2D_H