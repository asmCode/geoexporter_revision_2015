#include "MaterialProcessor.h"
#include "MaxHelpers.h"
#include "Code/Common/CustomParameter.h"
#include "code/SceneElements/Material.h"
#include <Utils/StringUtils.h>
#include <IO/Path.h>
#include <IGame/IGame.h>
#include <stdint.h>

MaterialProcessor::~MaterialProcessor()
{
	ParametersMap::iterator it;

	for (it = m_parameters.begin(); it != m_parameters.end(); ++it)
		delete it->second;
}

Material* MaterialProcessor::Create(IGameMaterial* maxMaterial)
{
	ExtractData(maxMaterial);

	return CreateMaterial();
}

void MaterialProcessor::ExtractData(IGameMaterial* maxMaterial)
{
	m_name = StringUtils::ToNarrow(maxMaterial->GetMaterialName());
	m_diffuseColor = MaxHelpers::GetPropertyVec3(maxMaterial->GetDiffuseData());
	m_specularColor = MaxHelpers::GetPropertyVec3(maxMaterial->GetSpecularData());
	m_opacity = MaxHelpers::GetPropertyFloat(maxMaterial->GetOpacityData());
	m_glossiness = MaxHelpers::GetPropertyFloat(maxMaterial->GetGlossinessData()) * 64.0f;
	m_specularLevel = MaxHelpers::GetPropertyFloat(maxMaterial->GetSpecularLevelData()) * 1.0f;

	for (int i = 0; i < maxMaterial->GetNumberOfTextureMaps(); i++)
	{
		IGameTextureMap* maxTexrture = maxMaterial->GetIGameTextureMap(i);
		if (maxTexrture == NULL)
			continue;

		int slot = maxTexrture->GetStdMapSlot();
		std::string textureName = Path(StringUtils::ToNarrow(maxTexrture->GetBitmapFileName())).GetFilename();

		switch (slot)
		{
		case ID_DI:
			m_diffuseTextureName = textureName;
			break;

		case ID_BU:
			m_normalMapTextureName = textureName;
			break;
		}
	}

	// TODO: extract user parameters
}

Material* MaterialProcessor::CreateMaterial()
{
	Material* material = new Material;

	material->Name = m_name;

	if (m_diffuseTextureName.empty() &&
		m_normalMapTextureName.empty() &&
		m_specularLevel == 0.0f)
	{
		material->ShaderName = "SolidDiff";
		material->AddParameter("u_color", "vec3", StringUtils::ToString(m_diffuseColor));
	}
	else if (m_diffuseTextureName.empty() &&
		m_normalMapTextureName.empty() &&
		m_specularLevel > 0.0f)
	{
		material->ShaderName = "SolidDiffSpec";
		material->AddParameter("u_color", "vec3", StringUtils::ToString(m_diffuseColor));
		material->AddParameter("u_specularLevel", "float", StringUtils::ToString(m_specularLevel));
		material->AddParameter("u_glossiness", "float", StringUtils::ToString(m_glossiness));
	}
	else if (!m_diffuseTextureName.empty() &&
		m_normalMapTextureName.empty() &&
		m_specularLevel == 0.0f)
	{
		material->ShaderName = "TexDiff";
		material->AddParameter("u_diffTex", "texture", m_diffuseTextureName);
	}
	else if (
		!m_diffuseTextureName.empty() &&
		m_normalMapTextureName.empty() &&
		m_specularLevel > 0.0f)
	{
		material->ShaderName = "TexDiffSpec";
		material->AddParameter("u_diffTex", "texture", m_diffuseTextureName);
		material->AddParameter("u_specularLevel", "float", StringUtils::ToString(m_specularLevel));
		material->AddParameter("u_glossiness", "float", StringUtils::ToString(m_glossiness));
	}
	else if	(
		!m_diffuseTextureName.empty() &&
		!m_normalMapTextureName.empty() &&
		m_specularLevel > 0.0f)
	{
		// tymczasowy zwykly tex diff specular
		material->ShaderName = "TexDiffSpec";
		material->AddParameter("u_diffTex", "texture", m_diffuseTextureName);
		material->AddParameter("u_specularLevel", "float", StringUtils::ToString(m_specularLevel));
		material->AddParameter("u_glossiness", "float", StringUtils::ToString(m_glossiness));
	}

	return material;
}

