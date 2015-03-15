#include "Light.h"
#include "../../MaxHelpers.h"
#include <XML/XmlWriter.h>
#include <IGame/IGame.h>

namespace SceneElements
{
	Light::Light() :
		m_brightness(0.0)
	{

	}

	Light* Light::ExtractFromNode(IGameScene* igScene, IGameNode* node)
	{
		if (node == NULL)
			return NULL;

		IGameObject* igObject = node->GetIGameObject();
		if (igObject == NULL)
			return NULL;

		if (igObject->GetIGameType() != IGameObject::ObjectTypes::IGAME_LIGHT)
			return NULL;

		IGameLight* igLight = dynamic_cast<IGameLight*>(igObject);
		if (igLight == NULL)
			return NULL;

		SceneElements::Light* light = new SceneElements::Light();

		if (igLight->GetLightType() == IGameLight::LightType::IGAME_OMNI)
			light->m_type = "point";
		else if (igLight->GetLightType() == IGameLight::LightType::IGAME_DIR)
			light->m_type = "directional";
		light->m_color = MaxHelpers::GetPropertyVec3(igLight->GetLightColor());
		light->m_brightness = MaxHelpers::GetPropertyFloat(igLight->GetLightMultiplier());

		node->ReleaseIGameObject();

		return light;
	}

	std::string Light::Serialize()
	{
		std::stringstream ss;

		XmlWriter xml(&ss, 0);
		xml.OpenElement("Light");
		xml.WriteAttribute("type", m_type);
		xml.WriteAttribute("color", StringUtils::ToString(m_color));
		xml.WriteAttribute("brightness", StringUtils::ToString(m_brightness));
		xml.CloseElement();

		return ss.str();
	}
}

