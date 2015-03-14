#pragma once

#include <string>

class CustomParameter;

namespace Serializers
{
	class CustomParameterSerializer
	{
	public:
		static std::string Serialize(const CustomParameter& customParameter);
	};
}
