#include "Colt.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <Core/Engine.h>

using namespace std;

Colt::Colt()
{
}

Colt::Colt(glm::vec3 centre, float _width, int type)
{
	this->centre = centre;

	start = centre;
	width = _width;

	indices = {
		0, 1, 2,		1, 3, 2,

	};

	if (type == 1) {
		Bordura* b1 = new Bordura(centre + glm::vec3(width/2, 0, -width), 'R');
		Bordura* b2 = new Bordura(centre + glm::vec3(width / 2, 0, 0));
		b.push_back(b1);
		b.push_back(b2);

		borduri.push_back(b1->CreateMesh("b1", b1->vertices, b1->indices));
		borduri.push_back(b2->CreateMesh("b2", b2->vertices, b2->indices));
	
		vertices = {
			VertexFormat(glm::vec3(centre.x - width / 2, centre.y,  centre.z), glm::vec3(0.5)),
			VertexFormat(glm::vec3(centre.x + width / 2, centre.y,  centre.z), glm::vec3(0.5)),
			VertexFormat(glm::vec3(centre.x - width / 2, centre.y,  centre.z - width), glm::vec3(0.5)),
			VertexFormat(glm::vec3(centre.x + width / 2, centre.y,  centre.z - width), glm::vec3(0.5)),
		};
	}
	else {
		Bordura* b1 = new Bordura(centre + glm::vec3(0, 0, width / 2), 'R');
		Bordura* b2 = new Bordura(centre + glm::vec3(-width, 0, width / 2));
		b.push_back(b1);
		b.push_back(b2);

		borduri.push_back(b1->CreateMesh("b1", b1->vertices, b1->indices));
		borduri.push_back(b2->CreateMesh("b2", b2->vertices, b2->indices));
		vertices = {
			VertexFormat(glm::vec3(centre.x, centre.y,  centre.z - width / 2), glm::vec3(0.5)),
			VertexFormat(glm::vec3(centre.x, centre.y,  centre.z + width / 2), glm::vec3(0.5)),
			VertexFormat(glm::vec3(centre.x - width, centre.y,  centre.z - width/2), glm::vec3(0.5)),
			VertexFormat(glm::vec3(centre.x - width, centre.y,  centre.z + width/2), glm::vec3(0.5)),
		};
	}
	finish = centre + glm::vec3(-width / 2, 0, -width / 2);
}

Colt::~Colt()
{
}

Mesh* Colt::CreateStrada(const char *name)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

	// set vertex normal attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	Mesh* strada = new Mesh(name);
	strada->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	strada->vertices = vertices;
	strada->indices = indices;
	return strada;
}

void Colt::RenderStrada(Mesh* colt, Shader *shader, const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {

	glm::mat4 b1Matrix = glm::mat4(1);
	glm::mat4 b2Matrix = glm::mat4(1);

	size = 0;

	GLint location;
	if (!colt || !shader || !shader->GetProgramID())
		return;

	
	glUseProgram(shaders["VertexColor"]->program);

	location = glGetUniformLocation(shaders["VertexColor"]->GetProgramID(), "View");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	location = glGetUniformLocation(shaders["VertexColor"]->GetProgramID(), "Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	location = glGetUniformLocation(shaders["VertexColor"]->GetProgramID(), "Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	borduri[0]->Render();
	borduri[1]->Render();


	glUseProgram(shader->program);

	location = glGetUniformLocation(shader->GetProgramID(), "Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "View");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "Time");
	glUniform1f(location, (GLfloat)abs(sin(Engine::GetElapsedTime())));

	colt->Render();
}
