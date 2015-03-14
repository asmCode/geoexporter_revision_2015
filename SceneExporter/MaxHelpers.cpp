#include "MaxHelpers.h"
#include <IGame/IGame.h>
#include <stddef.h>

float MaxHelpers::GetPropertyFloat(IGameProperty* gameProperty, float defaultValue)
{
	float value;

	if (gameProperty != NULL && gameProperty->GetPropertyValue(value))
		return value;
	else
		return defaultValue;
}

sm::Vec3 MaxHelpers::GetPropertyVec3(IGameProperty* gameProperty, sm::Vec3 defaultValue)
{
	Point3 value;

	if (gameProperty != NULL && gameProperty->GetPropertyValue(value))
		return sm::Vec3(value.x, value.y, value.z);
	else
		return defaultValue;
}

sm::Vec4 MaxHelpers::GetPropertyVec4(IGameProperty* gameProperty, sm::Vec4 defaultValue)
{
	Point4 value;

	if (gameProperty != NULL && gameProperty->GetPropertyValue(value))
		return sm::Vec4(value.x, value.y, value.z, value.w);
	else
		return defaultValue;
}
