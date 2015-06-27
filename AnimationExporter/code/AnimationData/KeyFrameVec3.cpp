#include "KeyFrameVec3.h"
#include <stdio.h>

KeyFrameVec3::KeyFrameVec3(float time, float x, float y, float z)
	: KeyFrameBase(time),
	m_x(x),
	m_y(y),
	m_z(z)
{
}

std::string KeyFrameVec3::Serialize()
{
	char text[1024];
	sprintf(text, "<Key t=\"%f\" v=\"%f,%f,%f\" />", m_time, m_x, m_y, m_z);
	return text;
}

