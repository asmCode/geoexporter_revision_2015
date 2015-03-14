#include "Parameter.h"

Parameter::Parameter(const std::string& name, float value)
{
}

Parameter::Parameter(const std::string& name, const sm::Vec3& value)
{
}

Parameter::Parameter(const std::string& name, const sm::Vec4& value)
{
}

float Parameter::GetFloat() const
{
	return Value.Float;
}

const sm::Vec3& Parameter::GetVec3() const
{
	return Value.Vec3;
}

const sm::Vec4& Parameter::GetVec4() const
{
	return Value.Vec4;
}

std::string Parameter::GetName() const
{
	return m_name;
}

ParameterType Parameter::GetType() const
{
	return m_type;
}
