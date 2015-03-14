#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include <XML/XmlWriter.h>
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

void GameObject::SetFromNode(IGameNode* node)
{
	if (node == NULL)
		return;

	m_transform = ExtractTransform(node);
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

	if (m_transform != NULL)
		xml.CreateElementInline(m_transform->Serialize());

	xml.CloseElement();

	return ss.str();
}

Transform* GameObject::ExtractTransform(IGameNode* node)
{
	if (node == NULL)
		return NULL;

	Transform* transform = new Transform();
	transform->SetFromNode(node);

	return transform;
}

