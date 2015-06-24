#pragma once

#include "KeyFrameBase.h"

class KeyFrameVec4 : public KeyFrameBase
{
public:
	KeyFrameVec4(float time);

	std::string Serialize();
};

