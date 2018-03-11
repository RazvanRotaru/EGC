#include "Object2DE.h"
#include "Transform2D.h"

#include <Core/Engine.h>

Mesh* Object2DE::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2DE::CreateTriangle(std::string name, glm::vec3 Centre, float l, glm::vec3 color, bool fill)
{
	glm::vec3 centre = Centre;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(centre, color),
		VertexFormat(centre + glm::vec3(0, 2 * l / 3, 0) , glm::vec3(0, 0, 0.8)),
		VertexFormat(centre + glm::vec3(-l / 2, -l / 3, 0), color),
		VertexFormat(centre + glm::vec3(l / 2, -l / 3 , 0), color)
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices =
	{
		0, 1, 2,
		0, 2, 3,
		1, 2, 3
	};

	if (!fill) {
		triangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
		//indices.push_back(1);
		//	indices.push_back(2);
	}

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Object2DE::CreatePlatform(std::string name, glm::vec3 leftBottomCorner, float l, float h, float rot, glm::vec3 color, bool reflect)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(l , 0, 0), color),
		VertexFormat(corner + glm::vec3(l , h, 0), color),
		VertexFormat(corner + glm::vec3(0 , h, 0), color)
	};

	Mesh* platform = new Mesh(name);
	std::vector<unsigned short> indices =
	{
		0, 1, 2,
		0, 2, 3
	};

	indices.push_back(0);
	//indices.push_back(1);
	indices.push_back(2);

	platform->InitFromData(vertices, indices);
	return platform;
}


Mesh* Object2DE::CreateAsteroid(std::string name, glm::vec3 Centre, float r, glm::vec3 color, bool fill)
{
	glm::vec3 corner = Centre;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(-r, 0, 0), color),
		VertexFormat(corner + glm::vec3(-r * sqrt(2) / 2, -r * sqrt(2) / 2, 0), color),
		VertexFormat(corner + glm::vec3(0, -r, 0), color),

		VertexFormat(corner + glm::vec3(r * sqrt(2) / 2, -r * sqrt(2) / 2, 0), color),
		VertexFormat(corner + glm::vec3(r, 0, 0), color),
		VertexFormat(corner + glm::vec3(r * sqrt(2) / 2, r * sqrt(2) / 2, 0), color),
		VertexFormat(corner + glm::vec3(0, r, 0), color),
		VertexFormat(corner + glm::vec3(-r * sqrt(2) / 2, r * sqrt(2) / 2, 0), color),



	};

	Mesh* asteroid = new Mesh(name);
	std::vector<unsigned short> indices =
	{
		1, 2, 0,
		2, 3, 0,
		3, 4, 0,
		4, 5, 0,
		5, 6, 0,
		6, 7, 0,
		7, 8, 0,
		8, 1, 0
	};

	if (!fill) {
		asteroid->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
		//indices.push_back(1);
		indices.push_back(2);
	}

	asteroid->InitFromData(vertices, indices);
	return asteroid;
}
