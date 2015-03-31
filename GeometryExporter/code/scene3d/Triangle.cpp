#include "Triangle.h"
#include <memory>

namespace FuturisEngine
{
	namespace Export
	{
		Triangle::Triangle(
			uint16_t vertexIndex[3],
			uint16_t normalIndex[3],
			uint16_t coord1Index[3])
		{
			if (vertexIndex != NULL)
				memcpy(m_vertexIndex, vertexIndex, sizeof(uint16_t) * 3);

			if (normalIndex != NULL)
				memcpy(m_normalIndex, normalIndex, sizeof(uint16_t) * 3);

			if (coord1Index != NULL)
				memcpy(m_coord1Index, coord1Index, sizeof(uint16_t) * 3);
		}

		const uint16_t* Triangle::GetVertexIndex() const
		{
			return m_vertexIndex;
		}

		const uint16_t* Triangle::GetNormalIndex() const
		{
			return m_normalIndex;
		}

		const uint16_t* Triangle::GetCoordIndex() const
		{
			return m_coord1Index;
		}
	}
}

