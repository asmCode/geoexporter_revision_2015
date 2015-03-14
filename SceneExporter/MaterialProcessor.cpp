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
	m_glossiness = MaxHelpers::GetPropertyFloat(maxMaterial->GetGlossinessData());
	m_specularLevel = MaxHelpers::GetPropertyFloat(maxMaterial->GetSpecularLevelData());

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
		m_normalMapTextureName.empty())
	{
		material->ShaderName = "SolidColorDiffuse";
		material->AddParameter("u_color", "vec3", StringUtils::ToString(m_diffuseColor));
	}
	else if (
		!m_diffuseTextureName.empty() &&
		m_normalMapTextureName.empty())
	{
		material->ShaderName = "SolidColor";
		material->AddParameter("u_color", "vec3", StringUtils::ToString(m_diffuseColor));
		material->AddParameter("u_diffTex", "texture", m_diffuseTextureName);
	}

	return material;
}

