#pragma once

#include <string>

class IGameMesh;

class Mesh
{
public:
	Mesh();
	~Mesh() {};

	void SetFromMesh(IGameMesh* mesh);

	std::string Serialize();

private:
	std::string m_meshName;
	std::string m_texName;
};

