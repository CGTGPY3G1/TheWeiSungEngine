#include "AttackerIdentityScript.h"
#include "GameObject.h"
#include "Transform2D.h"
AttackerIdentityScript::AttackerIdentityScript() : ScriptableComponent() {
}

AttackerIdentityScript::AttackerIdentityScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

AttackerIdentityScript::~AttackerIdentityScript() {
}

const AttackerInfo AttackerIdentityScript::GetInfo() const {
	return info;
}

void AttackerIdentityScript::Start() {
}

void AttackerIdentityScript::Reset() {
	SetAttacker(std::weak_ptr<GameObject>(), "");
}

void AttackerIdentityScript::SetAttackerName(const std::string & name) {
	info.name = name;
}

const std::string AttackerIdentityScript::GetAttackerName() const {
	return info.name;
}

void AttackerIdentityScript::SetAttacker(std::weak_ptr<GameObject> attacker, const std::string & name) {
	valid = (attacker.use_count() > 0);
	info.iD = valid ? attacker.lock()->GetCombinedObjectID() : 0;
	info.gameObject = attacker;
	info.name = name;
}

const std::weak_ptr<GameObject> AttackerIdentityScript::GetAttacker() const {
	return info.gameObject;
}

const unsigned int AttackerIdentityScript::GetAttackerID() const {
	return info.iD;
}

const bool AttackerIdentityScript::IsValid() const {
	return valid && info.iD != 0;
}

int AttackerIdentityScript::GetSortOrder() {
	const static int order = TypeInfo::ScriptSortOrder<AttackerIdentityScript>();
	return order;
}

AttackerInfo::AttackerInfo(std::weak_ptr<GameObject> attacker, const std::string & attackerName, const unsigned int & attackerID) : gameObject(attacker), name(attackerName), iD(attackerID) {
}

const bool AttackerInfo::IsAlive() const {
	return gameObject.use_count() > 0;
}

const Vector2 AttackerInfo::GetPosition() {
	return gameObject.use_count() > 0 ? gameObject.lock()->GetComponent<Transform2D>().lock()->GetPosition() : Vector2();
}

bool operator==(const AttackerInfo & lhs, const AttackerInfo & rhs) {
	return lhs.iD == rhs.iD;
}

bool operator==(const AttackerInfo & lhs, const unsigned int & rhs) {
	return lhs.iD == rhs;
}

bool operator==(const unsigned int & lhs, const AttackerInfo & rhs) {
	return lhs == rhs.iD;
}
