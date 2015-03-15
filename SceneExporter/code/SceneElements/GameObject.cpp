#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include <XML/XmlWriter.h>
#include <IGame/IGame.h>
#include <sstream>

GameObject::GameObject() :
	m_transform(NULL),
	m_mesh(NULL),
	m_camera(NULL),
	m_light(NULL)
{

}

GameObject::~GameObject()
{
	if (m_transform != NULL)
		delete m_transform;

	if (m_mesh != NULL)
		delete m_mesh;

	if (m_camera != NULL)
		delete m_camera;

	if (m_light != NULL)
		delete m_light;
}

void GameObject::SetFromNode(IGameScene* igScene, IGameNode* igNode)
{
	if (igNode == NULL)
		return;

	m_name = StringUtils::ToNarrow(igNode->GetName());

	m_transform = ExtractTransform(igNode);
	m_mesh = SceneElements::Mesh::ExtractFromNode(igScene, igNode);
	m_camera = SceneElements::Camera::ExtractFromNode(igScene, igNode);
	m_light = SceneElements::Light::ExtractFromNode(igScene, igNode);
}

bool GameObject::IsEmpty() const
{
	return m_mesh == NULL && m_camera == NULL && m_light == NULL;
}

std::string GameObject::Serialize()
{
	std::stringstream ss;

	XmlWriter xml(&ss, 0);
	xml.OpenElement("GameObject");
	xml.WriteAttribute("name", m_name);

	if (m_transform != NULL)
		xml.CreateElementInline(m_transform->Serialize());

	if (m_mesh != NULL)
		xml.CreateElementInline(m_mesh->Serialize());

	if (m_camera != NULL)
		xml.CreateElementInline(m_camera->Serialize());

	if (m_light != NULL)
		xml.CreateElementInline(m_light->Serialize());

	xml.CloseElement();

	return ss.str();
}

Transform* GameObject::ExtractTransform(IGameNode* igNode)
{
	if (igNode == NULL)
		return NULL;

	Transform* transform = new Transform();
	transform->SetFromNode(igNode);

	return transform;
}

