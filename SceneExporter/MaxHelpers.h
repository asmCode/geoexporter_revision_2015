#pragma once

#include <Math/Vec3.h>
#include <Math/Vec4.h>

class IGameProperty;

class MaxHelpers
{
public:
	static float GetPropertyFloat(IGameProperty* gameProperty, float defaultValue = 0.0f);
	static sm::Vec3 GetPropertyVec3(IGameProperty* gameProperty, sm::Vec3 defaultValue = sm::Vec3(0, 0, 0));
	static sm::Vec4 GetPropertyVec4(IGameProperty* gameProperty, sm::Vec4 defaultValue = sm::Vec4(0, 0, 0, 0));
};

