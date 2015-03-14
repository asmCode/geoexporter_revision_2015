#pragma once

#include <string>

class IGameNode;
class IGameScene;

namespace SceneElements
{
	class Mesh
	{
	public:
		Mesh();
		~Mesh() {};

		static Mesh* ExtractFromNode(IGameScene* igScene, IGameNode* node);

		std::string Serialize();

	private:
		std::string m_meshName;
		std::string m_materialName;
	};
}

