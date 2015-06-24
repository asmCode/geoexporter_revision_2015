#pragma once

#include "KeyFrameBase.h"

class KeyFrameVec3 : public KeyFrameBase
{
public:
	KeyFrameVec3(float time);

	std::string Serialize();
};

