#include "AnimationCurve.h"
#include "KeyFrameBase.h"
#include <XML/XmlWriter.h>
#include <stdio.h>
#include <stdint.h>

AnimationCurve::AnimationCurve(
	PropertyType propertyType,
	const std::string& objectName,
	const std::string& componentName,
	const std::string& propertyName)
	:
	m_propertyType(propertyType),
	m_objectName(objectName),
	m_componentName(componentName),
	m_propertyName(propertyName)
{
}

AnimationCurve::~AnimationCurve()
{
	for (uint32_t i = 0; i < m_keyframes.size(); i++)
		delete m_keyframes[i];
}

std::string AnimationCurve::Serialize()
{
	XmlWriter xml;
	xml.OpenElement("Anim");
	xml.WriteAttribute("propertyType", PropertyTypeToString(m_propertyType));
	xml.WriteAttribute("target_name", m_objectName);
	xml.WriteAttribute("component_name", m_componentName);
	xml.WriteAttribute("property_name", m_propertyName);

	for (uint32_t i = 0; i < m_keyframes.size(); i++)
		xml.CreateElementInline(m_keyframes[i]->Serialize());

	xml.CloseElement();

	return xml.GetAsText();
}

void AnimationCurve::AddKeyframe(KeyFrameBase* keyframe)
{
	m_keyframes.push_back(keyframe);
}

std::string AnimationCurve::PropertyTypeToString(PropertyType propertyType)
{
	switch (propertyType)
	{
	case PropertyType_Float: return "float";
	case PropertyType_Int: return "int";
	case PropertyType_Vec3: return "vec3";
	case PropertyType_Quat: return "quat";
	}

	return "";
}

