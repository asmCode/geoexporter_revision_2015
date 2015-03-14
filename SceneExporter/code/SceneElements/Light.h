#pragma once

#include <string>

class IGameLight;

class Light
{
public:
	Light();
	~Light() {};

	void SetFromLight(IGameLight* light);

	std::string Serialize();

private:
};

