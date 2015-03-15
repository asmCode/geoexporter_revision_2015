#pragma once

#include <Math/Vec3.h>
#include <string>

class IGameNode;
class IGameScene;

namespace SceneElements
{
	class Light
	{
	public:
		Light();
		~Light() {};

		static Light* ExtractFromNode(IGameScene* igScene, IGameNode* node);

		std::string Serialize();

	private:
		std::string m_type;
		sm::Vec3 m_color;
		float m_brightness;
	};
}

