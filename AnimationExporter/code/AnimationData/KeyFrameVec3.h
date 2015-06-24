#pragma once

#include "KeyFrameBase.h"

class KeyFrameVec3 : public KeyFrameBase
{
public:
	KeyFrameVec3(float time, float x, float y, float z);

	std::string Serialize();

private:
	float m_x;
	float m_y;
	float m_z;
};

