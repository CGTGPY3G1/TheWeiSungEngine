#include "Transform2D.h"
#include "GameObject.h"

Transform2D::Transform2D() : Component(), sf::Transformable() {
}

Transform2D::Transform2D(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {
}

Transform2D::~Transform2D() {

}

Vector2 Transform2D::GetPosition() {
	if(dirty) CalculateWorldTransform();
	return Vector2(world.getPosition());
}

void Transform2D::SetPosition(const Vector2 & newPosition) {
	setPosition(sf::Vector2f(newPosition.x, newPosition.y));
	SetDirty();
}

float Transform2D::GetRotation() {
	if(dirty) CalculateWorldTransform();
	return getRotation();
}

void Transform2D::SetRotation(const float & newRotation) {
	setRotation(newRotation);
	SetDirty();
}

Vector2 Transform2D::GetScale() {
	if(dirty) CalculateWorldTransform();
	return Vector2(getScale());
}

void Transform2D::SetScale(const Vector2 & newScale) {
	SetDirty();
	setScale(newScale.x, newScale.y);
}

Vector2 Transform2D::GetForward() {
	if(dirty) CalculateWorldTransform();
	return Vector2(1, 0).RotateInDegrees(GetRotation());
}

Vector2 Transform2D::GetRight() {
	return GetForward().Flip90(true);
}

void Transform2D::Move(Vector2 toMove) {
	move(TypeConversion::ConvertToSFVector2f(toMove.RotatedInDegrees(getRotation())));
	SetDirty();
}

void Transform2D::Translate(const Vector2 & translation) {
	sf::Vector2f newTranslation = this->getTransform().transformPoint(translation.x, translation.y);
	setPosition(newTranslation);
	SetDirty();
}

void Transform2D::Rotate(const float & angle) {
	rotate(angle);
	SetDirty();
}

const sf::Transform & Transform2D::GetLocalTransform() {
	return getTransform();
}

const sf::Transform & Transform2D::GetWorldTransform() {
	if(dirty) CalculateWorldTransform();
	return world.getTransform();
}

int Transform2D::GetChildCount() {
	return children.size();
}

std::weak_ptr<Transform2D> Transform2D::GetChild(const unsigned int & index) {
	return index < children.size() ? children[index] : std::weak_ptr<Transform2D>();
}

void Transform2D::AddChild(std::weak_ptr<Transform2D> child) {
	children.push_back(child);
}

std::weak_ptr<Transform2D> Transform2D::GetParent() {
	return parent;
}

void Transform2D::SetParent(std::weak_ptr<Transform2D> newParent) {
	parent = newParent;
	if(!parent.expired()) {
		std::shared_ptr<GameObject> g = gameObject.lock();
		parent.lock()->AddChild(g->GetComponent<Transform2D>());
	}
}

//void Transform2D::HandleMessage(const Message<Vector2> & message) {
//	switch(message.type) {
//	case MESSAGE_TYPE_POSITION_UPDATE:
//		SetPosition(message.data);
//	default:
//		break;
//	}
//}
//
//void Transform2D::HandleMessage(const Message<float> & message) {
//	switch(message.type) {
//	case MESSAGE_TYPE_ROTATION_UPDATE:
//		SetRotation(message.data);
//	default:
//		break;
//	}
//}

void Transform2D::CalculateWorldTransform() {
	if(!parent.expired()) {
		std::shared_ptr<Transform2D> p = parent.lock();
		world = Transformable(p->world);
		world.move(getPosition());
		world.rotate(getRotation());
		world.scale(getScale());
	}
	else {
		world = Transformable(*this);
	}
	dirty = false;
}

void Transform2D::SetDirty() {
	if(!dirty) {
		dirty = true;
		for(size_t i = 0; i < children.size(); i++) {
			std::shared_ptr<Transform2D> c = children[i].lock();
			c->SetDirty();
		}
	}
}
