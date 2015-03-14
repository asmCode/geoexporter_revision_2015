#pragma once

#include "../Common/CustomParameter.h"
#include <string>
#include <vector>

class CustomSceneElement
{
public:
	std::string Type;
	std::string Id;
	std::string MeshName;
	std::string MaterialName;

	std::vector<CustomParameter> Parameters;
};
