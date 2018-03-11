#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateTriangle(std::string name, glm::vec3 Corner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateAsteroid(std::string name, glm::vec3 Centre, float radius, glm::vec3 color, bool fill = false);
	Mesh* CreatePlatform(std::string name, glm::vec3 leftBottomCorner, float length, float height, float rotation, glm::vec3 color, bool fill = false);

}

