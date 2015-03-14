#pragma once

#include <Math/Vec3.h>
#include <vector>
#include <map>
#include <string>

class CustomParameter;

class Material
{
public:
	typedef std::map<std::string, CustomParameter*> ParametersMap;

	std::string Name;
	std::string ShaderName;

	~Material();

	void AddParameter(
		const std::string& name,
		const std::string& type,
		const std::string& value);

	const ParametersMap& GetParameters() const;

private:
	ParametersMap m_parameters;
};

