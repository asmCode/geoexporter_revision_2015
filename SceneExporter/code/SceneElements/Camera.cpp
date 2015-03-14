#include "Camera.h"
#include "../../MaxHelpers.h"
#include <XML/XmlWriter.h>
#include <IGame/IGame.h>

namespace SceneElements
{
	Camera::Camera() :
		m_fov(0.0f),
		m_nearPlane(0.0f),
		m_farPlane(0.0f)
	{

	}

	Camera* Camera::ExtractFromNode(IGameScene* igScene, IGameNode* node)
	{
		if (node == NULL)
			return NULL;

		IGameObject* igObject = node->GetIGameObject();
		if (igObject == NULL)
			return NULL;

		if (igObject->GetIGameType() != IGameObject::ObjectTypes::IGAME_CAMERA)
			return NULL;

		IGameCamera* igCamera = dynamic_cast<IGameCamera*>(igObject);
		if (igCamera == NULL)
			return NULL;

		SceneElements::Camera* camera = new SceneElements::Camera();
		camera->m_fov = MaxHelpers::GetPropertyFloat(igCamera->GetCameraFOV());
		camera->m_nearPlane = MaxHelpers::GetPropertyFloat(igCamera->GetCameraNearClip());
		camera->m_farPlane = MaxHelpers::GetPropertyFloat(igCamera->GetCameraFarClip());
		
		node->ReleaseIGameObject();

		return camera;
	}

	std::string Camera::Serialize()
	{
		std::stringstream ss;

		XmlWriter xml(&ss, 0);
		xml.OpenElement("Camera");
		xml.WriteAttribute("fov", StringUtils::ToString(m_fov));
		xml.WriteAttribute("near_plane", StringUtils::ToString(m_nearPlane));
		xml.WriteAttribute("far_plane", StringUtils::ToString(m_farPlane));
		xml.CloseElement();

		return ss.str();
	}
}

