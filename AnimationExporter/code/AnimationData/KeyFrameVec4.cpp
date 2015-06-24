#include "KeyFrameVec4.h"

KeyFrameVec4::KeyFrameVec4(float time, float x, float y, float z, float w)
	: KeyFrameBase(time),
	m_x(x),
	m_y(y),
	m_z(z),
	m_w(w)
{
}

std::string KeyFrameVec4::Serialize()
{
	char text[1024];
	sprintf(text, "<key t=\"%f\" v=\"%f,%f,%f,%f\" />", m_time, m_x, m_y, m_z, m_w);
	return text;
}

