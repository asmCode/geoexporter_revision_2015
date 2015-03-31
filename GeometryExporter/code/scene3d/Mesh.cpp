#include "Mesh.h"
#include <IO/BinaryWriter.h>
#include <Utils/Log.h>
#include <limits>

namespace FuturisEngine
{
	namespace Export
	{
		Mesh::Mesh(const std::string& name) :
			m_name(name)
		{
		}

		void Mesh::AddVertexTriangle(uint16_t a, uint16_t b, uint16_t c)
		{
			m_vertexTriangles.push_back(a);
			m_vertexTriangles.push_back(b);
			m_vertexTriangles.push_back(c);
		}

		void Mesh::AddNormalTriangle(uint16_t a, uint16_t b, uint16_t c)
		{
			m_normalTriangles.push_back(a);
			m_normalTriangles.push_back(b);
			m_normalTriangles.push_back(c);
		}

		void Mesh::AddCoord1Triangle(uint16_t a, uint16_t b, uint16_t c)
		{
			m_coord1Triangles.push_back(a);
			m_coord1Triangles.push_back(b);
			m_coord1Triangles.push_back(c);
		}

		void Mesh::AddVertex(float x, float y, float z)
		{
			m_vertices.push_back(sm::Vec3(x, y, z));
		}

		void Mesh::AddNormal(float x, float y, float z)
		{
			m_normals.push_back(sm::Vec3(x, y, z));
		}

		void Mesh::AddCoord1(float u, float v)
		{
			m_coords1.push_back(sm::Vec2(u, v));
		}

		bool Mesh::Validate() const
		{
			if (m_vertices.size() > USHRT_MAX)
			{
				Log::LogT("Mesh %s has to many vertices: %u", m_name.c_str(), m_vertices.size());
				return false;
			}

			if (m_normals.size() > USHRT_MAX)
			{
				Log::LogT("Mesh %s has to many normals: %u", m_name.c_str(), m_normals.size());
				return false;
			}

			if (m_coords1.size() > USHRT_MAX)
			{
				Log::LogT("Mesh %s has to many coords: %u", m_name.c_str(), m_coords1.size());
				return false;
			}

			return true;
		}

		void Mesh::Save(std::ostream& stream)
		{
			Log::LogT("Saving mesh: %s", m_name.c_str());
			Log::LogT("Vertices: %u", m_vertices.size());
			Log::LogT("Normals: %u", m_normals.size());
			Log::LogT("Coords1: %u", m_coords1.size());

			assert(m_normalTriangles.size() == 0 || m_normalTriangles.size() == m_vertexTriangles.size());
			assert(m_coord1Triangles.size() == 0 || m_coord1Triangles.size() == m_vertexTriangles.size());

			uint8_t vertexChannel = 0;

			if (m_vertices.size() > 0)
				vertexChannel |= FlagVertexChannelVertices;
			if (m_normals.size() > 0)
				vertexChannel |= FlagVertexChannelNormals;
			if (m_coords1.size() > 0)
				vertexChannel |= FlagVertexChannelCoords1;

			BinaryWriter writer(&stream);

			writer.Write(m_name);
			writer.Write<uint8_t>(vertexChannel);

			if ((vertexChannel & FlagVertexChannelVertices) > 0)
			{
				writer.Write((uint16_t)m_vertices.size());
				writer.Write(m_vertices.data(), (uint32_t)(m_vertices.size() * sizeof(sm::Vec3)));
			}

			if ((vertexChannel & FlagVertexChannelNormals) > 0)
			{
				writer.Write((uint16_t)m_normals.size());
				writer.Write(m_normals.data(), (uint32_t)(m_normals.size() * sizeof(sm::Vec3)));
			}

			if ((vertexChannel & FlagVertexChannelCoords1) > 0)
			{
				writer.Write((uint16_t)m_coords1.size());
				writer.Write(m_coords1.data(), (uint32_t)(m_coords1.size() * sizeof(sm::Vec2)));
			}

			uint32_t trianglesCount = m_vertexTriangles.size() / 3;
			writer.Write<uint16_t>((uint16_t)trianglesCount);

			for (uint32_t i = 0; i < trianglesCount; i++)
			{
				if ((vertexChannel & FlagVertexChannelVertices) > 0)
					writer.Write(m_vertexTriangles.data() + i * 3, sizeof(uint16_t) * 3);

				if ((vertexChannel & FlagVertexChannelNormals) > 0)
					writer.Write(m_normalTriangles.data() + i * 3, sizeof(uint16_t) * 3);

				if ((vertexChannel & FlagVertexChannelCoords1) > 0)
					writer.Write(m_coord1Triangles.data() + i * 3, sizeof(uint16_t) * 3);
			}

			/*Log::LogT("Mesh:");
			for (uint32_t i = 0; i < trianglesCount; i++)
			{
				Log::LogT("[%d, %d, %d], [%d, %d, %d], [%d, %d]",
					m_vertexTriangles[i * 3 + 0], m_vertexTriangles[i * 3 + 1], m_vertexTriangles[i * 3 + 2],
					m_normalTriangles[i * 3 + 0], m_normalTriangles[i * 3 + 1], m_normalTriangles[i * 3 + 2],
					m_coord1Triangles[i * 3 + 0], m_coord1Triangles[i * 3 + 1], m_coord1Triangles[i * 3 + 2]);
			}*/

			/*
			Log::LogT("Mesh:");
			for (uint32_t i = 0; i < trianglesCount * 3; i++)
			{
				Log::LogT("%d: [%d, %d, %d]",
					i, m_vertexTriangles[i], m_normalTriangles[i], m_coord1Triangles[i]);
			}
			*/
		}
	}
}

