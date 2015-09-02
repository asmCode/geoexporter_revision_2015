#pragma once

#include <Math\Vec2.h>
#include <Math\Vec3.h>
#include <vector>
#include <string>
#include <stdint.h>

namespace FuturisEngine
{
	namespace Export
	{
		class Triangle;

		class Mesh
		{
		public:
			Mesh(const std::string& name);

			void AddVertexTriangle(uint16_t a, uint16_t b, uint16_t c);
			void AddVertex(float x, float y, float z);

			void AddNormalTriangle(uint16_t a, uint16_t b, uint16_t c);
			void AddNormal(float x, float y, float z);

			void AddTangentTriangle(uint16_t a, uint16_t b, uint16_t c);
			void AddTangent(float x, float y, float z);

			void AddCoord1Triangle(uint16_t a, uint16_t b, uint16_t c);
			void AddCoord1(float u, float v);

			bool Validate() const;
			void Save(std::ostream& stream);

		private:
			static const uint8_t FlagVertexChannelVertices = 1 << 0;
			static const uint8_t FlagVertexChannelNormals = 1 << 1;
			static const uint8_t FlagVertexChannelTangents = 1 << 2;
			static const uint8_t FlagVertexChannelCoords1 = 1 << 3;

			std::string m_name;

			std::vector<uint16_t> m_vertexTriangles;
			std::vector<sm::Vec3> m_vertices;

			std::vector<uint16_t> m_normalTriangles;
			std::vector<sm::Vec3> m_normals;

			std::vector<uint16_t> m_tangentTriangles;
			std::vector<sm::Vec3> m_tangents;

			std::vector<uint16_t> m_coord1Triangles;
			std::vector<sm::Vec2> m_coords1;
		};
	}
}

