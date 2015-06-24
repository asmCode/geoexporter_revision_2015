#pragma once

#include "KeyFrameBase.h"

class KeyFrameVec4 : public KeyFrameBase
{
public:
	KeyFrameVec4(float time, float x, float y, float z, float w);

	std::string Serialize();

private:
	float m_x;
	float m_y;
	float m_z;
	float m_w;
};

