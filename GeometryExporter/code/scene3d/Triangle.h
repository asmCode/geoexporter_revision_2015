#pragma once

#include <stdint.h>

namespace FuturisEngine
{
	namespace Export
	{
		class Triangle
		{
		public:
			Triangle(
				uint16_t vertexIndex[3],
				uint16_t normalIndex[3],
				uint16_t coord1Index[3]);

			const uint16_t* GetVertexIndex() const;
			const uint16_t* GetNormalIndex() const;
			const uint16_t* GetCoordIndex() const;

		private:
			uint16_t m_vertexIndex[3];
			uint16_t m_normalIndex[3];
			uint16_t m_coord1Index[3];
		};
	}
}

