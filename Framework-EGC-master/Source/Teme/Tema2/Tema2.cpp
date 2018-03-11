#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

#define MAX_COLISIONS 5

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{

	polygonMode = GL_FILL;
	
	translateZ = 0;
	translateY = 0;
	translateX = 0;
	int cnt = 0;
	s.push_back(new Strada(glm::vec3(0,0.03,0), 10)); cnt++;
	s.push_back(new Colt(s[cnt - 1]->finish, 10, 1)); cnt++;
	s.push_back(new Strada(s[cnt - 1]->finish, 10, 'R')); cnt++;
	s.push_back(new Colt(s[cnt - 1]->finish, 10, 2)); cnt++;

	Bordura * halp;
	halp = new Bordura(50, 250, 250, glm::vec3(0.5, 1.2, 0.9));
	cer = halp->CreateMesh("cer", halp->vertices, halp->indices);
	halp = new Bordura(0.01, 250, 250, glm::vec3(-0.15, 0.7, 0));
	iarba = halp->CreateMesh("iarba", halp->vertices, halp->indices);

	for (int i = 0; i < s.size(); i++)
		meshes["strada"+i] = s[i]->CreateStrada("strada" + i);

	bmw_seria3_din_2007 = new Masina(glm::vec3(0));

	camera = new Camera();
	glm::vec3 centre = bmw_seria3_din_2007->centre;
	float width = bmw_seria3_din_2007->width;
	float angle = bmw_seria3_din_2007->angle;
	
	glm::vec4 add = Transform3D::RotateOY(-angle) * glm::vec4(0, 0, 4 * width, 1);
	glm::vec3 cam_centre = centre + glm::vec3(add.x , 4 * bmw_seria3_din_2007->length, add.z);
	camera->Set(cam_centre, centre, glm::vec3(0, 1, 0));

	{
		Shader *shader = new Shader("ShaderTema");
		shader->AddShader("Source/Teme/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}


void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	if (!gameOver && ready) {
	
		glLineWidth(3);
		glPointSize(5);
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

		glm::mat4 modelMatrix = glm::mat4(1);
		for (int i = 0; i < s.size(); i++) {
			s[i]->RenderStrada(meshes["strada" + i], shaders["ShaderTema"], modelMatrix, getCamera()->GetViewMatrix(), getProjectionMatrix());
		}
		bmw_seria3_din_2007->moveForward(translateZ);
		bmw_seria3_din_2007->moveSideways(translateX, -wheel);
		glm::vec3 centre = bmw_seria3_din_2007->centre;
		float width = bmw_seria3_din_2007->width;
		float angle = bmw_seria3_din_2007->angle;

		if (camType == 1) {

			glm::vec4 add = Transform3D::RotateOY(-angle) * glm::vec4(0, 0, -width / 2 + width / 2.5, 1);
			glm::vec3 cam_centre = centre + glm::vec3(add.x, bmw_seria3_din_2007->length, add.z);

			add = Transform3D::RotateOY(-angle) * glm::vec4(0, -0.3, -width / 2, 1);

			camera->Set(cam_centre,
				centre + glm::vec3(add.x, bmw_seria3_din_2007->length + add.y, add.z),
				glm::vec3(0, 1, 0));
		}
		else {
			glm::vec4 add = Transform3D::RotateOY(-angle) * glm::vec4(0, 0, 4 * width, 1);
			glm::vec3 cam_centre = centre + glm::vec3(add.x, 4 * bmw_seria3_din_2007->length, add.z);
			camera->Set(cam_centre, centre, glm::vec3(0, 1, 0));
		}

		RenderMesh(cer, shaders["ShaderTema"], glm::mat4(1));
		RenderMesh(iarba, shaders["ShaderTema"], glm::mat4(1));

		bmw_seria3_din_2007->RenderMasina(shaders["VertexNormal"], wheel, deltaTimeSeconds, getCamera()->GetViewMatrix(), getProjectionMatrix());
		VerifyColision();
		isGameOver();
	}
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::isGameOver() {

	if (colisions >= MAX_COLISIONS) {
		gameOver = true;
		cout << "MASINA ASTA NU SE CONDUCE SINGURA" << endl;
	}

	if (bmw_seria3_din_2007->centre.z < s[s.size() - 1]->finish.z) {
		gameOver = true;
		cout << "FELICITARI! Ai obtiunt " << points << " puncte" << endl;
	}
}

void Tema2::VerifyColision() {
	
	for (int j = 0; j < s.size(); j++) {
		
		for (int i = 0; i < s[j]->obs.size(); i++) {
			if (bmw_seria3_din_2007->Colides(s[j]->obs[i])) {
				cout << "SHAMANKA \n";
				points--;
				colisions++;
			}
			if (bmw_seria3_din_2007->centre.z < s[j]->obs[i]->centre.z &&
				s[j]->dir == 'F' && !s[j]->aquired[i]) {
				points++;
				s[j]->aquired[i] = true;
			}
			if (bmw_seria3_din_2007->centre.x < s[j]->obs[i]->centre.x &&
				s[j]->dir != 'F' && !s[j]->aquired[i]) {
				points++;
				s[j]->aquired[i] = true;
			}	
		}
		for (int i = 0; i < s[j]->b.size(); i++) {
			if (bmw_seria3_din_2007->Colides(s[j]->b[i])) {
				cout << " SAMANTA \n";
				points--;
				colisions++;
			}
		}
	}
}

Camera* Tema2::getCamera() {
	return camera;
}

glm::mat4 Tema2::getProjectionMatrix() {
	return projectionMatrix;
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(getCamera()->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(getProjectionMatrix()));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix)
{
	GLint location;
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO : get shader location for uniform mat4 "Model"
	location = glGetUniformLocation(shader->GetProgramID(), "Model");

	// TODO : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// TODO : get shader location for uniform mat4 "View"
	location = glGetUniformLocation(shader->GetProgramID(), "View");

	// TODO : set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = getCamera()->GetViewMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// TODO : get shader location for uniform mat4 "Projection"
	location = glGetUniformLocation(shader->GetProgramID(), "Projection");

	// TODO : set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = getProjectionMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "Time");
	glUniform1f(location, (GLfloat) abs(sin(Engine::GetElapsedTime())));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// add key press event
	if (window->KeyHold(GLFW_KEY_W)) {
		translateZ -= 0.01f * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		translateX -=  deltaTime;
		wheel = 1;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		translateZ += 0.01f * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		translateX += deltaTime;
		wheel = 2;
	}

}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}

	if (key == GLFW_KEY_C) {
		camType = 1 - camType;
	}

	if (key == GLFW_KEY_H)
		ready = true;
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_A) {
		wheel = 0;
	}

	if (key == GLFW_KEY_D) {
		wheel = 0;
	}

	if (key == GLFW_KEY_W) {
		translateZ = 0.f;
	}

	if (key == GLFW_KEY_S) {
		translateZ = 0.f;
	}

}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
