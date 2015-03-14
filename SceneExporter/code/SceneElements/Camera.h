#pragma once

#include <string>

class IGameCamera;

class Camera
{
public:
	Camera();
	~Camera() {};

	void SetFromCamera(IGameCamera* camera);

	std::string Serialize();

private:
};

