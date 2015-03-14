#pragma once

#include <string>

class IGameNode;
class IGameScene;

namespace SceneElements
{
	class Camera
	{
	public:
		Camera();
		~Camera() {};

		static Camera* ExtractFromNode(IGameScene* igScene, IGameNode* node);

		std::string Serialize();

	private:
		float m_fov;
		float m_nearPlane;
		float m_farPlane;
	};
}

