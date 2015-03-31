#include "Model.h"
#include "Mesh.h"
#include <IO/BinaryWriter.h>
#include <string>
#include <fstream>

namespace FuturisEngine
{
	namespace Export
	{
		Model::Model()
		{
		}

		Model::~Model()
		{
			for (uint32_t i = 0; i < m_meshes.size(); i++)
				delete m_meshes[i];
		}

		void Model::AddMesh(Mesh* mesh)
		{
			m_meshes.push_back(mesh);
		}

		void Model::Save(const std::string& filename)
		{
			std::ofstream fileStream(filename, std::ios::binary);

			BinaryWriter writer(&fileStream);

			writer.Write<uint32_t>((uint32_t)m_meshes.size());
			for (uint32_t i = 0; i < m_meshes.size(); i++)
				m_meshes[i]->Save(fileStream);

			fileStream.close();
		}
	}
}

