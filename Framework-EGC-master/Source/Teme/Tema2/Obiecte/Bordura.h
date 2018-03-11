#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include "MyObject.h"

class Bordura : public MyObject
{
public:
	Bordura(glm::vec3 centre, const char dir = 'F', const glm::vec3 color = glm::vec3(0.1, 0.5, 0.4));
	Bordura(float height, float length, float width, glm::vec3 color, glm::vec3 centre = glm::vec3(0));
	~Bordura();
};