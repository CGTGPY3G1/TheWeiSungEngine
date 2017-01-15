//#include "XMLDocument.h"
//#include <sstream>
//#include "Debug.h"

//
//XMLNode * XMLDocument::GetNodeByName(const string & nodeName, XMLNode * node) {
//	XMLNode * toReturn = nullptr;
//	if(node != nullptr) {
//		if(node->name() == nodeName) return node;
//		toReturn = GetNodeByName(nodeName, node->first_node());
//		if(toReturn == nullptr) toReturn = GetNodeByName(nodeName, node->next_sibling());
//	}
//	return toReturn;
//}
//
//XMLNode * XMLDocument::GetNodeByNameAndAttribute(const string & nodeName, const string & attrName, const string & attribValue, XMLNode * node) {
//	XMLNode * toReturn = nullptr;
//	if(node != nullptr) {
//		XMLAttribute * attr = node->first_attribute(attrName.c_str());
//		if(node->name() == nodeName && ((attr != NULL) && (attr->value() == attribValue))) return node;
//		toReturn = GetNodeByNameAndAttribute(nodeName, attrName, attribValue, node->first_node());
//		if(toReturn == nullptr) toReturn = GetNodeByNameAndAttribute(nodeName, attrName, attribValue, node->next_sibling());
//	}
//	return toReturn;
//}
//
//string XMLDocument::GetAttributeValue(const std::string & attributeName, XMLNode * node) {
//	string attributeValue = "";
//	if(node == nullptr) Debug::Log("Null Parent Node!", DebugMessageType::DEBUG_TYPE_FAILURE_MEDIUM);
//	else {
//		XMLAttribute * attribute = node->first_attribute(attributeName.c_str());
//		if(attribute == nullptr) Debug::Log("Attribute <" + attributeName + "> not found!", DebugMessageType::DEBUG_TYPE_FAILURE_MEDIUM);
//		else attributeValue = attribute->value();
//	}
//	return attributeValue;
//}
