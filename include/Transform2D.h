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
class Transform2D : public Component, protected sf::Transformable {
public:
	Transform2D();
	Transform2D(std::weak_ptr<GameObject> gameObject);
	~Transform2D();
	const ComponentType Type() const override { return COMPONENT_TRANSFORM_2D; }
	Vector2 GetPosition();
	Vector2 GetLocalPosition();
	void SetPosition(const Vector2 & newPosition);
	float GetRotation();
	float GetLocalRotation();
	void SetRotation(const float & newRotation);
	Vector2 GetScale();
	Vector2 GetLocalScale();
	void SetScale(const Vector2 & newScale);
	Vector2 GetForward();
	Vector2 GetRight();
	Vector2 TransformToWorldPoint(const Vector2 & point);
	Vector2 TransformToLocalPoint(const Vector2 & point);
	Vector2 TransformToWorldDirection(const Vector2 & direction);
	Vector2 TransformToLocalDirection(const Vector2 & direction);
	void Move(Vector2 toMove);
	void Translate(const Vector2 & translation);
	void Rotate(const float & angle);
	void Scale(const float & scaleBy);
	void Scale(const Vector2 & scale);
	const sf::Transform & GetLocalTransform();
	const sf::Transform & GetWorldTransform();
	const sf::Transform & GetWorldToLocalTransform();
	const sf::Transform & GetLocalToWorldTransform();

	unsigned int GetChildCount();
	std::weak_ptr<Transform2D> GetChild(const unsigned int & index);
	void AddChild(std::weak_ptr<Transform2D> child);
	std::weak_ptr<Transform2D> GetParent();
	void SetParent(std::weak_ptr<Transform2D> newParent);
	/*void HandleMessage(const Message<Vector2> & message) override;
	void HandleMessage(const Message<float> & message) override;*/
	const std::string GetName() const override { return "Transform2D"; }

	template <class Archive>
	void load(Archive & ar) {

	}

	template <class Archive>
	void save(Archive & ar) const {
		Component::save(ar);
		ar(cereal::make_nvp("Position", worldPosition),
		   cereal::make_nvp("Rotation", worldRotation),
		   cereal::make_nvp("Scale", worldScale));
	}
protected:
	bool dirty = true;
	void CalculateWorldTransform();
	void SetDirty();
	sf::Transform world, world2Local, local2World;
	Vector2 worldPosition = Vector2(0, 0);
	Vector2 worldScale = Vector2(1, 1);
	float worldRotation;
	std::weak_ptr<Transform2D> parent;
	std::vector<std::weak_ptr<Transform2D>> children;
};

#endif // !WS_TRANSFORM_2D_H

