#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include "Strada.h"
#include "../Transform3D.h"
#include "../Tema2.h"

class Colt : public Strada
{
public:

	Colt();
	Colt(glm::vec3 centre, float width, const int type);
	~Colt();

	Mesh * CreateStrada(const char * name = "colt");
	void RenderStrada(Mesh* colt, Shader *shader, const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);

};