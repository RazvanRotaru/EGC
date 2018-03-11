#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include "Bordura.h"
#include "../Transform3D.h"

class Strada : public SimpleScene
{
public:

	Strada();
	Strada(glm::vec3 centre,float width, const char dir = 'F');
	virtual ~Strada();

	virtual Mesh * CreateStrada(const char * name = "strada");
	virtual void RenderStrada(Mesh* strada, Shader *shader, const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);



	std::vector<Mesh *> borduri;
	std::vector<Mesh *> obstacole;
	bool aquired[3];
	Mesh * bordura;
	std::vector<Bordura *> b;
	std::vector<Bordura *> obs;
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	glm::vec3 centre, start, finish;
	bool bordura_creata;
	char dir;
	int cnt, size;
	float width;
};