#include "Bordura.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Bordura::Bordura(float height, float length, float width, glm::vec3 color, glm::vec3 centre) {
	
	this->length = length;
	this->width = width;
	this->height = height;

	this->centre = centre;

	vertices = {
		VertexFormat(glm::vec3(centre.x - length / 2, centre.y - height / 2,  centre.z + width / 2), color),
		VertexFormat(glm::vec3(centre.x + length / 2, centre.y - height / 2,  centre.z + width / 2), color),
		VertexFormat(glm::vec3(centre.x - length / 2, centre.y - height / 2,  centre.z - width / 2), color),
		VertexFormat(glm::vec3(centre.x + length / 2, centre.y - height / 2,  centre.z - width / 2), color),
		VertexFormat(glm::vec3(centre.x - length / 2, centre.y + height / 2,  centre.z + width / 2), color),
		VertexFormat(glm::vec3(centre.x + length / 2, centre.y + height / 2,  centre.z + width / 2), color),
		VertexFormat(glm::vec3(centre.x - length / 2, centre.y + height / 2,  centre.z - width / 2), color),
		VertexFormat(glm::vec3(centre.x + length / 2, centre.y + height / 2,  centre.z - width / 2), color),
	};

	indices =
	{
		0, 1, 2,		1, 3, 2,
		2, 3, 7,		2, 7, 6,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
	};


}
Bordura::Bordura(glm::vec3 centre, char dir, glm::vec3 color)
{
	length = 0.5;
	height = 0.5;
	width = 10;

	if (dir == 'F') {
		length = 0.5;
		width = 10;
		centre = centre + glm::vec3(0, height / 2, -width / 2);
	}
	else {
		length = 10;
		width = 0.5;
		centre = centre + glm::vec3(-length / 2, height / 2, 0);
	}

	
	this->centre = centre;

	vertices = {
		VertexFormat(glm::vec3(centre.x - length / 2, centre.y - height / 2,  centre.z + width / 2), color, color),
		VertexFormat(glm::vec3(centre.x + length / 2, centre.y - height / 2,  centre.z + width / 2), color, color),
		VertexFormat(glm::vec3(centre.x - length / 2, centre.y - height / 2,  centre.z - width / 2), color, color),
		VertexFormat(glm::vec3(centre.x + length / 2, centre.y - height / 2,  centre.z - width / 2), color, color),
		VertexFormat(glm::vec3(centre.x - length / 2, centre.y + height / 2,  centre.z + width / 2), color, color),
		VertexFormat(glm::vec3(centre.x + length / 2, centre.y + height / 2,  centre.z + width / 2), color, color),
		VertexFormat(glm::vec3(centre.x - length / 2, centre.y + height / 2,  centre.z - width / 2), color, color),
		VertexFormat(glm::vec3(centre.x + length / 2, centre.y + height / 2,  centre.z - width / 2), color, color),
	};

	indices =
	{
		0, 1, 2,		1, 3, 2,
		2, 3, 7,		2, 7, 6,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
	};

}

Bordura::~Bordura()
{
}