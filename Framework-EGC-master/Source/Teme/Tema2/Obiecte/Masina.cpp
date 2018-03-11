#include "Masina.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Masina::Masina(glm::vec3 centre)
{
	for (int i = 0; i < 4; i++) {
		roti.push_back(new Mesh("r+i"));
		roti[i]->LoadMesh(RESOURCE_PATH::MODELS + "Props", "oildrum.obj");
	}
	length = 1;
	height = 0.5;
	width = 2;

	angle = 0;
	a = 0.f; 
	v = 0;


	vertices = {
		VertexFormat(glm::vec3(centre.x - length / 2, centre.y - height / 2,  centre.z + width / 2), glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3(centre.x + length / 2, centre.y - height / 2,  centre.z + width / 2), glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3(centre.x - length / 2, centre.y - height / 2,  centre.z - width / 2), glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3(centre.x + length / 2, centre.y - height / 2,  centre.z - width / 2), glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3(centre.x - length / 2, centre.y + height / 2,  centre.z + width / 2), glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3(centre.x + length / 2, centre.y + height / 2,  centre.z + width / 2), glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3(centre.x - length / 2, centre.y + height / 2,  centre.z - width / 2), glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3(centre.x + length / 2, centre.y + height / 2,  centre.z - width / 2), glm::vec3(1, 0, 0)),
		

		VertexFormat(glm::vec3(centre.x - length * 0.8 / 2, centre.y + height * 3 / 4 - height / 4,  centre.z + width / 6 + width / 4), glm::vec3(1)),
		VertexFormat(glm::vec3(centre.x + length * 0.8 / 2, centre.y + height * 3 / 4 - height / 4,  centre.z + width / 6 + width / 4), glm::vec3(1)),
		VertexFormat(glm::vec3(centre.x - length * 0.8 / 2, centre.y + height * 3 / 4 - height / 4,  centre.z + width / 6 - width / 4), glm::vec3(1)),
		VertexFormat(glm::vec3(centre.x + length * 0.8 / 2, centre.y + height * 3 / 4 - height / 4,  centre.z + width / 6 - width / 4), glm::vec3(1)),
		VertexFormat(glm::vec3(centre.x - length * 0.8 / 2, centre.y + height * 3 / 4 + height / 4,  centre.z + width / 6 + width / 4), glm::vec3(1)),
		VertexFormat(glm::vec3(centre.x + length * 0.8 / 2, centre.y + height * 3 / 4 + height / 4,  centre.z + width / 6 + width / 4), glm::vec3(1)),
		VertexFormat(glm::vec3(centre.x - length * 0.8 / 2, centre.y + height * 3 / 4 + height / 4,  centre.z + width / 6 - width / 4), glm::vec3(1)),
		VertexFormat(glm::vec3(centre.x + length * 0.8 / 2, centre.y + height * 3 / 4 + height / 4,  centre.z + width / 6 - width / 4), glm::vec3(1)),
	};

	indices =
	{
		0, 1, 2,		1, 3, 2,
		2, 3, 7,		2, 7, 6,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,

		8, 9, 10,		9, 11, 10,
		10, 11, 15,		10, 15, 14,
		9, 15, 11,		9, 13, 15,
		14, 15, 12,		15, 13, 12,
		8, 12, 9,		9, 12, 13,
		10, 14, 12,		8, 10, 12,
	};

	self = MyObject::CreateMesh("masina", vertices, indices);

}

Masina::~Masina()
{
}

void Masina::RenderMasina(Shader *shader, int wheel, float deltaT, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	matrixes.clear();
	glm::mat4 matricea;
	matricea *= Transform3D::Translate(0, height / 2 + 0.2, -width / 2);
	
	v += a * deltaT * 2;

	float x = v;
	float z = v;
	glm::vec4 add = Transform3D::RotateOY(-angle) * glm::vec4(0, 0, v, 1);

	tZ += add.z;
	tX += add.x;

	glm::mat4 b1Matrix = glm::mat4(1);
	b1Matrix *= Transform3D::Translate(tX, 0, tZ-width/2);
	b1Matrix *= Transform3D::RotateOY(-angle);
	b1Matrix *= Transform3D::Translate(-length / 2, 0.2 , - width/2 + 0.25);
	if (wheel == 1)
		if (v > 0)
			b1Matrix *= Transform3D::RotateOY(-M_PI / 6);
		else
			b1Matrix *= Transform3D::RotateOY(M_PI / 6);

	if (wheel == 2)
		if (v > 0)
			b1Matrix *= Transform3D::RotateOY(M_PI / 6);
		else
			b1Matrix *= Transform3D::RotateOY(-M_PI / 6);	
	b1Matrix *= Transform3D::RotateOX(tZ);
	b1Matrix *= Transform3D::RotateOZ(M_PI/2);
	b1Matrix *= Transform3D::Scale(0.5, 0.1, 0.5);

	glm::mat4 b2Matrix = glm::mat4(1);
	b2Matrix *= Transform3D::Translate(tX, 0, tZ-width/2);
	b2Matrix *= Transform3D::RotateOY(-angle);
	b2Matrix *= Transform3D::Translate(length / 2 + 0.1, 0.2, -width/ 2+ 0.25);
	if (wheel == 1)
		if (v > 0)
			b2Matrix *= Transform3D::RotateOY(-M_PI/6);
		else
			b2Matrix *= Transform3D::RotateOY(M_PI / 6);

	if (wheel == 2)
		if (v > 0)
			b2Matrix *= Transform3D::RotateOY(M_PI / 6);
		else
			b2Matrix *= Transform3D::RotateOY(-M_PI / 6);
	b2Matrix *= Transform3D::RotateOX(tZ);
	b2Matrix *= Transform3D::RotateOZ(M_PI/2);
	b2Matrix *= Transform3D::Scale(0.5, 0.1, 0.5);

	glm::mat4 b3Matrix = glm::mat4(1);
	b3Matrix *= Transform3D::Translate(tX, 0, tZ - width/2);
	b3Matrix *= Transform3D::RotateOY(-angle);
	b3Matrix *= Transform3D::Translate(-length / 2, 0.2, width / 2- 0.25);
	b3Matrix *= Transform3D::RotateOX(tZ);
	b3Matrix *= Transform3D::RotateOZ(M_PI/2);
	b3Matrix *= Transform3D::Scale(0.5, 0.1, 0.5);

	glm::mat4 b4Matrix = glm::mat4(1);
	b4Matrix *= Transform3D::Translate(tX, 0, tZ - width/2);
	b4Matrix *= Transform3D::RotateOY(-angle);
	b4Matrix *= Transform3D::Translate(length / 2 + 0.1, 0.2,  width/2-0.25);
	b4Matrix *= Transform3D::RotateOX(tZ);
	b4Matrix *= Transform3D::RotateOZ(M_PI/2);
	b4Matrix *= Transform3D::Scale(0.5, 0.1, 0.5);

	matrixes.push_back(b1Matrix);
	matrixes.push_back(b2Matrix);
	matrixes.push_back(b3Matrix);
	matrixes.push_back(b4Matrix);
	
	GLint location;

	glUseProgram(shaders["VertexColor"]->program);

	location = glGetUniformLocation(shader->GetProgramID(), "View");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	
	location = glGetUniformLocation(shader->GetProgramID(), "Model");

	for (int i = 0; i < 4; i++) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrixes[i]));

		roti[i]->Render();
	}

	matricea *= Transform3D::Translate(tX, 0, tZ);
	matricea *= Transform3D::RotateOY(-angle);
	
	centre = glm::vec3(tX, height / 2 + 0.2, -width / 2 + tZ);

	glUseProgram(shader->program);

	location = glGetUniformLocation(shader->GetProgramID(), "View");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matricea));

	self->Render();

}

void Masina::moveForward(float delta) {

	a = delta;
}

void Masina::moveSideways(float delta, float wheel) {
	if (v < 0)
		angle = -delta;
	angle = delta;
	wheel_angle = wheel;
}

//glm::vec3 centre = obj->centre;
//float len = obj->length;
//float wid = obj->width;
//
//float h = 0.5 * (this->width + wid);
//float w = 0.5 * (this->length + len);

bool Masina::Colides(MyObject *obj) {
	glm::vec3 centre = obj->centre;
	float wid = obj->length;
	float len = obj->width;
	
	float w = 0.5 * (this->width + wid);
	float h = 0.5 * (this->length + len);
	float dx = this->centre.x - centre.x;
	float dy = this->centre.z - centre.z;

	if (abs(dx) <= w && abs(dy) <= h && last_col != obj && v != 0) {
		v = 0;
		a = 0;
		last_col = obj;
		return true;
	}
	return false;
}