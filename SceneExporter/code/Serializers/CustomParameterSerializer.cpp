#include "CustomParameterSerializer.h"
#include "../Common/CustomParameter.h"
#include <XML/XmlWriter.h>
#include <sstream>

namespace Serializers
{
	std::string CustomParameterSerializer::Serialize(const CustomParameter& customParameter)
	{
		std::stringstream sstream;

		XmlWriter xml(&sstream, 0);

		xml.OpenElement("Parameter");
		xml.WriteAttribute("name", customParameter.GetName());
		xml.WriteAttribute("type", customParameter.GetType());
		xml.WriteAttribute("value", customParameter.GetValue());
		xml.CloseElement();

		return sstream.str();
	}
}
