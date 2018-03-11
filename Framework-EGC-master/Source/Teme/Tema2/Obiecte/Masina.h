#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include <Teme\Tema2\Obiecte\MyObject.h>
#include <Teme\Tema2\Obiecte\Strada.h>
#include <Teme\Tema2\Transform3D.h>
//#include "../Tema2.h"



class Masina : public MyObject
{
public:
	Masina(glm::vec3 centre);
	~Masina();

	void RenderMasina(Shader *shader, int wheel, float deltaT, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
	void moveForward(float delta);
	void moveSideways(float delta, float wheel);
	bool Colides(MyObject *obj);

	glm::vec3 centre;

	std::vector<Mesh*> roti;
	std::vector<glm::mat4> matrixes;
	Mesh* self;
	MyObject* last_col;

	float a, v, s, angle, wheel_angle;
	float tX, tY, tZ;
};