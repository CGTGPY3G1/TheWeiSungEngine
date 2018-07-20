#include "Transform2D.h"
#include "GameObject.h"
namespace WeiSungEngine {
	Transform2D::Transform2D() : Component(), sf::Transformable() {
		SetEnabled(true);
	}

	Transform2D::Transform2D(std::weak_ptr<GameObject> gameObject) : Component(gameObject), sf::Transformable() {
		SetEnabled(true);
	}

	Transform2D::~Transform2D() {

	}

	Vector2 Transform2D::GetPosition() {
		if (dirty) CalculateWorldTransform();
		return worldPosition;
	}

	Vector2 Transform2D::GetLocalPosition() {
		return getPosition();
	}

	void Transform2D::SetPosition(const Vector2 & newPosition) {
		if (!parent.expired()) setPosition(parent.lock()->TransformToLocalPoint(newPosition));
		else setPosition(newPosition.x, newPosition.y);
		SetDirty();
	}

	float Transform2D::GetRotation() {
		if (dirty) CalculateWorldTransform();
		return worldRotation;
	}

	float Transform2D::GetLocalRotation() {
		return getRotation();
	}

	void Transform2D::SetRotation(const float & newRotation) {
		Rotate(newRotation - GetRotation());
	}

	Vector2 Transform2D::GetScale() {
		if (dirty) CalculateWorldTransform();
		return worldScale;
	}

	Vector2 Transform2D::GetLocalScale() {
		return getScale();
	}

	void Transform2D::SetScale(const Vector2 & newScale) {
		SetDirty();
		setScale(newScale.x, newScale.y);
	}

	Vector2 Transform2D::GetForward() {
		if (dirty) CalculateWorldTransform();
		const float* matrix = GetWorldTransform().getMatrix();
		return Vector2(matrix[0], matrix[1]).Normalized();
	}

	Vector2 Transform2D::GetRight() {
		if (dirty) CalculateWorldTransform();
		const float* matrix = GetWorldTransform().getMatrix();
		return Vector2(matrix[4], matrix[5]).Normalized();
	}

	Vector2 Transform2D::TransformToWorldPoint(const Vector2 & point) {
		return GetLocalToWorldTransform().transformPoint(point.x, point.y);
	}

	Vector2 Transform2D::TransformToLocalPoint(const Vector2 & point) {
		return GetWorldToLocalTransform().transformPoint(point.x, point.y);
	}

	Vector2 Transform2D::TransformToWorldDirection(const Vector2 & direction) {
		const float* matrix = GetLocalToWorldTransform().getMatrix();
		return Vector2(matrix[0] * direction.x + matrix[1] * direction.y, matrix[4] * direction.x + matrix[5] * direction.y).Normalize();
	}

	Vector2 Transform2D::TransformToLocalDirection(const Vector2 & direction) {
		const float* matrix = GetWorldToLocalTransform().getMatrix();
		return Vector2(matrix[0] * direction.x + matrix[1] * direction.y, matrix[4] * direction.x + matrix[5] * direction.y).Normalize();
	}

	void Transform2D::Move(Vector2 toMove) {
		move(TypeConversion::ConvertToSFVector2f(toMove));
		SetDirty();
	}

	void Transform2D::Translate(const Vector2 & translation) {
		sf::Vector2f newTranslation = this->GetWorldToLocalTransform().transformPoint(translation.x, translation.y);
		setPosition(newTranslation);
		SetDirty();
	}

	void Transform2D::Rotate(const float & angle) {
		rotate(angle);
		SetDirty();
	}

	void Transform2D::Scale(const float & scaleBy) {
		SetScale(GetScale() * scaleBy);
	}

	void Transform2D::Scale(const Vector2 & toScale) {
		scale(TypeConversion::ConvertToSFVector2f(toScale));
		SetDirty();
	}

	const sf::Transform & Transform2D::GetLocalTransform() {
		return getTransform();
	}

	const sf::Transform & Transform2D::GetWorldTransform() {
		if (dirty) CalculateWorldTransform();
		return world;
	}

	const sf::Transform & Transform2D::GetWorldToLocalTransform() {
		if (dirty) CalculateWorldTransform();
		return world2Local;
	}

	const sf::Transform & Transform2D::GetLocalToWorldTransform() {
		if (dirty) CalculateWorldTransform();
		return local2World;
	}

	unsigned int Transform2D::GetChildCount() {
		return (unsigned int)children.size();
	}

	std::weak_ptr<Transform2D> Transform2D::GetChild(const unsigned int & index) {
		return index < children.size() ? children[index] : std::weak_ptr<Transform2D>();
	}

	void Transform2D::AddChild(std::weak_ptr<Transform2D> child) {
		children.push_back(child);
		SetDirty();
	}

	std::weak_ptr<Transform2D> Transform2D::GetParent() {
		return parent;
	}

	void Transform2D::SetParent(std::weak_ptr<Transform2D> newParent) {
		Vector2 oldPosition = GetPosition();
		float oldRotation = GetRotation();
		parent = newParent;
		std::shared_ptr<GameObject> g = gameObject.lock();
		if (!parent.expired()) {
			std::shared_ptr<Transform2D> p = parent.lock();
			std::shared_ptr<GameObject> pg = p->GetGameObject().lock();
			SetPosition(oldPosition);
			SetRotation(oldRotation);
			p->AddChild(GetComponent<Transform2D>());
			g->combinedObjectID = pg->combinedObjectID;
		}
		else {
			g->combinedObjectID = g->objectID;
		}
		SetDirty();
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
		sf::Transform transform = sf::Transform::Identity;
		std::shared_ptr<Transform2D> node = parent.lock();
		worldRotation = getRotation();
		worldScale = getScale();
		if (node) {
			sf::Transform t = node->GetWorldTransform();
			transform *= t * transform;
			worldRotation += node->GetRotation();
			sf::Vector2f newScale = node->GetScale();
			worldScale.x *= newScale.x; worldScale.y *= newScale.y;
		}

		local2World = transform;
		world = transform * getTransform();
		world2Local = world.getInverse();

		worldPosition = world * sf::Vector2f();
		dirty = false;
	}

	void Transform2D::SetDirty() {
		if (!dirty) {
			dirty = true;
			for (size_t i = 0; i < children.size(); i++) {
				std::shared_ptr<Transform2D> c = children[i].lock();
				c->SetDirty();
			}
		}
	}
}
