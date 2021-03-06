#pragma once

#include <string>
#include <vector>

class Transform;
namespace SceneElements
{
	class Mesh;
	class Camera;
	class Light;
}
class IGameNode;
class IGameScene;

class GameObject
{
public:
	GameObject();
	~GameObject();

	//
	void SetFromNode(IGameScene* igScene, IGameNode* igNode);

	// nie posiada zadnego komponentu poza Transform
	bool IsEmpty() const;

	//
	std::string Serialize();

private:
	std::string m_name;
	std::string m_parent;

	Transform* m_transform;
	SceneElements::Mesh* m_mesh;
	SceneElements::Camera* m_camera;
	SceneElements::Light* m_light;

	Transform* ExtractTransform(IGameNode* igNode);
};

