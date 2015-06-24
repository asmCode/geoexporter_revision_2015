#pragma once

#include <string>

class KeyFrameBase
{
public:
	KeyFrameBase(float time);

	virtual std::string Serialize() = 0;

protected:
	float m_time;
};

