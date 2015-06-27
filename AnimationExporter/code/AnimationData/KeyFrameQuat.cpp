#include "KeyFrameQuat.h"

KeyFrameQuat::KeyFrameQuat(float time, float a, float x, float y, float z)
	: KeyFrameBase(time),
	m_a(a),
	m_x(x),
	m_y(y),
	m_z(z)
{
}

std::string KeyFrameQuat::Serialize()
{
	char text[1024];
	sprintf(text, "<Key t=\"%f\" v=\"%f,%f,%f,%f\" />", m_time, m_a, m_x, m_y, m_z);
	return text;
}

