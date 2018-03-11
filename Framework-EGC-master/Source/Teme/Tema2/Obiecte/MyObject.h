#pragma once
#include <Core/GPU/Mesh.h>
#include <Component/SimpleScene.h>

class MyObject : public SimpleScene
{
public:
	MyObject();
	virtual ~MyObject();

	
	static Mesh * CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices);

	glm::vec3 centre;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	float length, height, width;
};