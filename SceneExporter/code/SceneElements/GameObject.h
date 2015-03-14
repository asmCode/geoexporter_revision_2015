#pragma once

#include <string>
#include <vector>

class Transform;
class Mesh;
class Camera;
class Light;
class IGameNode;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void SetFromNode(IGameNode* node);

	std::string Serialize();

private:
	std::string m_name;

	Transform* m_transform;
	Mesh* m_mesh;
	Camera* m_camera;
	Light* m_light;
};

