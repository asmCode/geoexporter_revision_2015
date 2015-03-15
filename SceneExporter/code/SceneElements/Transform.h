#pragma once

#include <Math/Vec3.h>
#include <Math/Vec4.h>
#include <string>

class IGameNode;

class Transform
{
public:
	Transform();
	~Transform() {};

	void SetFromNode(IGameNode* node);

	std::string Serialize();

	sm::Vec3 m_position;
	sm::Vec4 m_rotation;
	sm::Vec3 m_scale;
};

