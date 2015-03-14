#include "Material.h"
#include "../Common/CustomParameter.h"
#include <Utils/MapUtils.h>
#include <stdint.h>

/*
const std::string Material::ParameterNames::DiffuseColor = "DiffuseColor";
const std::string Material::ParameterNames::SpecularColor = "SpecularColor";
const std::string Material::ParameterNames::Opacity = "Opacity";
const std::string Material::ParameterNames::Glossiness = "Glossiness";
const std::string Material::ParameterNames::SpecularLevel = "SpecularLevel";
const std::string Material::ParameterNames::DiffuseTexture = "DiffuseTexture";
const std::string Material::ParameterNames::NormalMapTexture = "NormalMapTexture";
*/

Material::~Material()
{
	ParametersMap::iterator it;

	for (it = m_parameters.begin(); it != m_parameters.end(); ++it)
		delete it->second;
}

void Material::AddParameter(
	const std::string& name,
	const std::string& type,
	const std::string& value)
{
	CustomParameter* param = new CustomParameter(name, type, value);
	m_parameters[name] = param;
}

const Material::ParametersMap& Material::GetParameters() const
{
	return m_parameters;
}

