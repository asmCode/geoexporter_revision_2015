#pragma once

#include "../PropertyType.h"
#include <string>
#include <vector>

class KeyFrameBase;

class AnimationCurve
{
public:
	AnimationCurve(
		PropertyType propertyType,
		const std::string& objectName,
		const std::string& componentName,
		const std::string& propertyName);

	~AnimationCurve();

	std::string Serialize();

	void AddKeyframe(KeyFrameBase* keyframe);

private:
	PropertyType m_propertyType;
	std::string m_objectName;
	std::string m_componentName;
	std::string m_propertyName;

	std::vector<KeyFrameBase*> m_keyframes;

	static std::string PropertyTypeToString(PropertyType propertyType);
};

