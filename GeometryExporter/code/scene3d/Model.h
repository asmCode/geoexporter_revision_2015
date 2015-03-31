#pragma once

#include <vector>
#include <string>

namespace FuturisEngine
{
	namespace Export
	{
		class Mesh;

		class Model
		{
		public:
			Model();
			~Model();

			void AddMesh(Mesh* mesh);

			void Save(const std::string& filename);

		private:
			std::vector<Mesh*> m_meshes;
		};
	}
}

