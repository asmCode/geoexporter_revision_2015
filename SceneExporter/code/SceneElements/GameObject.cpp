#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"

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

}

std::string GameObject::Serialize()
{
	return "";
}

