#pragma once

#include <Math/Vec3.h>
#include <string>
#include <map>

class Material;
class IGameMaterial;
class CustomParameter;

// bierze material z maxa, sprawdza jakie ma parametry, tekstury i wypluwa
// material w formacie rozumianym przez FuturisEngine
class MaterialProcessor
{
public:
	~MaterialProcessor();

	Material* Create(IGameMaterial* maxMaterial);

private:
	typedef std::map<std::string, CustomParameter*> ParametersMap;

	std::string m_name;
	sm::Vec3 m_diffuseColor;
	sm::Vec3 m_specularColor;
	float m_opacity;
	float m_glossiness;
	float m_specularLevel;
	std::string m_diffuseTextureName;
	std::string m_normalMapTextureName;

	ParametersMap m_parameters;

	void ExtractData(IGameMaterial* maxMaterial);
	Material* CreateMaterial();
};

