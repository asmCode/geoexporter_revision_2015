#pragma once

#include "ParameterType.h"
#include <Math/Vec3.h>
#include <Math/Vec4.h>
#include <string>

class Parameter
{
public:
	Parameter(const std::string& name, float value);
	Parameter(const std::string& name, const sm::Vec3& value);
	Parameter(const std::string& name, const sm::Vec4& value);

	float GetFloat() const;
	const sm::Vec3& GetVec3() const;
	const sm::Vec4& GetVec4() const;

	std::string GetName() const;
	ParameterType GetType() const;

private:
	std::string m_name;
	ParameterType m_type;

	union ValueUnion
	{
		float Float;
		struct
		{
			sm::Vec3 Vec3;
			sm::Vec4 Vec4;
			std::string String;
		};
	} Value;
};
