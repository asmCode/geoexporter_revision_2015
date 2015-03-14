#pragma once

#include <string>

class CustomParameter
{
public:
	CustomParameter(
		const std::string& name,
		const std::string& type,
		const std::string& value);

	std::string GetName() const;
	std::string GetType() const;
	std::string GetValue() const;

private:
	std::string m_name;
	std::string m_type;
	std::string m_value;
};
