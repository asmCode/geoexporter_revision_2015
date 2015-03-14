#include "CustomParameter.h"

CustomParameter::CustomParameter(
	const std::string& name,
	const std::string& type,
	const std::string& value) :
	m_name(name),
	m_type(type),
	m_value(value)
{

}

std::string CustomParameter::GetName() const
{
	return m_name;
}

std::string CustomParameter::GetType() const
{
	return m_type;
}

std::string CustomParameter::GetValue() const
{
	return m_value;
}

