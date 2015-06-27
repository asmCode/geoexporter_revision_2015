#pragma once

#include "KeyFrameBase.h"

class KeyFrameQuat : public KeyFrameBase
{
public:
	KeyFrameQuat(float time, float a, float x, float y, float z);

	std::string Serialize();

private:
	float m_a;
	float m_x;
	float m_y;
	float m_z;
};

