#include "Strada.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <random>

#include <Core/Engine.h>

#define MAX_SIZE 10

using namespace std;

Strada::Strada() 
{
}

Strada::Strada(glm::vec3 centre, float _width, char dir)
{
	this->centre = centre;
	this->dir = dir;

	start = centre;
	size = 0;
	width = _width;

	indices = {
		0, 1, 2,		1, 3, 2,
		2, 3 ,4,		3, 5, 4,
		4, 5, 6,		5, 7, 6,
		6, 7, 8,		7, 9, 8,
		8, 9, 10,		9, 11, 10,
		10, 11, 12,		11, 13, 12,
		12, 13, 14,		13, 15, 14,
		14, 15, 16,		15, 17, 16,
		16, 17, 18,		17, 19, 18,	
	};

	while (size < MAX_SIZE) {
		
		if (dir == 'F') {
			if (size != MAX_SIZE - 1) {
				Bordura* b1 = new Bordura(centre + glm::vec3(-width / 2, 0, 0));
				Bordura* b2 = new Bordura(centre + glm::vec3(width / 2, 0, 0));
				b.push_back(b1);
				b.push_back(b2);
				if (!bordura_creata) {
					bordura = b1->CreateMesh("bordura", b1->vertices, b1->indices);
					bordura_creata = true;
				}
				if (size == 3 || size == 5 || size == 7) {
					Bordura *o = new Bordura(4, 1.5, 1.5,glm::vec3(1, 0.5, 0), centre + glm::vec3(-width * 0.6 /2 + rand() % (int) width * 0.6, 0, 0));
					obs.push_back(o);
					obstacole.push_back(o->CreateMesh("obstacol", o->vertices, o->indices));
				}

			}
			vertices.push_back(VertexFormat(centre - glm::vec3(width / 2, 0, 0), glm::vec3(0.5, 0.5, 0.5)));
			vertices.push_back(VertexFormat(centre + glm::vec3(width / 2, 0, 0), glm::vec3(0.5, 0.5, 0.5)));

			size++;
			if (size != MAX_SIZE)
				centre += glm::vec3(0, 0, -10);
		}
		else {
			if (size != MAX_SIZE - 1) {
				Bordura* b1 = new Bordura(centre + glm::vec3(0, 0, -width / 2), 'R');
				Bordura* b2 = new Bordura(centre + glm::vec3(0, 0, width / 2), 'R');
				b.push_back(b1);
				b.push_back(b2);

				if (!bordura_creata) {
					bordura = b1->CreateMesh("bordura", b1->vertices, b1->indices);
					bordura_creata = true;
				}
				if (size == 3 || size == 5 || size == 7) {
					Bordura *o = new Bordura(4, 1.5, 1.5, glm::vec3(1, 0.5, 0), centre + glm::vec3(0, 0, -width * 0.6 /2 + rand() % (int)width* 0.6));
					obs.push_back(o);
					obstacole.push_back(o->CreateMesh("obstacol"+size, o->vertices, o->indices));
				}

			}
			vertices.push_back(VertexFormat(centre - glm::vec3(0, 0, width / 2), glm::vec3(0.5, 0.5, 0.5)));
			vertices.push_back(VertexFormat(centre + glm::vec3(0, 0, width / 2), glm::vec3(0.5, 0.5, 0.5)));

			size++;
			if (size != MAX_SIZE)
				centre += glm::vec3(-10, 0, 0);
		}
	}
	finish = centre;
}

Strada::~Strada()
{
}

Mesh* Strada::CreateStrada(const char *name)
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

void Strada::RenderStrada(Mesh* strada, Shader *shader, const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {

	glm::mat4 b1Matrix = glm::mat4(1);
	glm::mat4 b2Matrix = glm::mat4(1);
	if (dir == 'F') {
		b2Matrix *= Transform3D::Translate(width, 0, 0);
	}
	else {
		b2Matrix *= Transform3D::Translate(0, 0, width);
	}
	GLint location;
	if (!strada || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shaders["VertexNormal"]->program);

	location = glGetUniformLocation(shaders["VertexNormal"]->GetProgramID(), "View");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	location = glGetUniformLocation(shaders["VertexNormal"]->GetProgramID(), "Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	location = glGetUniformLocation(shaders["VertexNormal"]->GetProgramID(), "Model");
	size = 0;

	while (size < MAX_SIZE - 1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(b1Matrix));
		bordura->Render();

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(b2Matrix));
		bordura->Render();

		if (dir == 'F') {
			b1Matrix *= Transform3D::Translate(0, 0, -b[0]->width);
			b2Matrix *= Transform3D::Translate(0, 0, -b[0]->width);
		}
		else {
			b1Matrix *= Transform3D::Translate(-b[0]->length, 0, 0);
			b2Matrix *= Transform3D::Translate(-b[0]->length, 0, 0);
		}
		size++;
	}

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

	for (int i = 0; i < obstacole.size(); i++) {
		obstacole[i]->Render();
	}

	glUseProgram(shader->program);

	location = glGetUniformLocation(shader->GetProgramID(), "View");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "Time");
	glUniform1f(location, (GLfloat)abs(sin(Engine::GetElapsedTime())));

	location = glGetUniformLocation(shader->GetProgramID(), "Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	
	strada->Render();

}
