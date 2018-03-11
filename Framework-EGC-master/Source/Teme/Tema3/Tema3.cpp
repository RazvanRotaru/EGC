#include "Tema3.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::Init()
{
	const string textureLoc = "Source/Teme/Tema3/Textures/";
	
	init_lght = light_pos = glm::vec3(0, 10, 0);
	plane_color = glm::vec3(0.749f, 0.847f, 0.847f);
	light_color = glm::vec3(1, 0.43f, 0.72f);
	material_shininess = 12;
	anglstep = 0;
	no_layers = 6;
	outline = false;
	toon = true;

	// Load textures
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "Banana.jpg").c_str(), GL_REPEAT);
		mapTextures["kd"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "Banana2.jpg").c_str(), GL_REPEAT);
		mapTextures["ks"] = texture;
	}

	// Load meshes
	
	{
		Mesh* mesh = new Mesh("banana");
		mesh->LoadMesh("Source/Teme/Tema3", "Banana.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("floor");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderBanana");
		shader->AddShader("Source/Teme/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader *shader = new Shader("ShaderOutline");
		shader->AddShader("Source/Teme/Tema3/Shaders/OutlineVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema3/Shaders/OutlineFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void Tema3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::Update(float deltaTimeSeconds)
{
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		anglstep += M_PI / 21 * 0.01;
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
		light_pos = init_lght + glm::vec3(30, 0, -20) *sin(anglstep);
		modelMatrix = glm::translate(modelMatrix, light_pos);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderOutline"], modelMatrix, false, light_color);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.015f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-100, 1, 100));
		modelMatrix = glm::rotate(modelMatrix, (glm::mediump_float)M_PI / 2, glm::vec3(1, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -300, 150));
		RenderBanana(meshes["banana"], shaders["ShaderBanana"], modelMatrix, mapTextures["kd"], mapTextures["ks"], toon);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		
		if (outline) RenderSimpleMesh(meshes["banana"], shaders["ShaderOutline"], modelMatrix, outline);
		
		glDisable(GL_CULL_FACE);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		RenderBanana(meshes["floor"], shaders["ShaderBanana"], modelMatrix, nullptr, nullptr, false, plane_color);
	}
}

void Tema3::FrameEnd()
{
}

void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, bool outline, const glm::vec3 &color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;
	glUseProgram(shader->program);

	int clr = glGetUniformLocation(shader->program, "color");
	glUniform3fv(clr, 1, glm::value_ptr(color));

	glUniform1i(glGetUniformLocation(shader->program, "outline"), (int)outline);
	glUniform1f(glGetUniformLocation(shader->program, "puls"), (float)abs(sin(Engine::GetElapsedTime())));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::RenderBanana(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1, Texture2D* texture2, bool toon, const glm::vec3 & color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(light_position, 1, glm::value_ptr(light_pos));

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(eye_position, 1, glm::value_ptr(eyePosition));

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int ms = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(ms, material_shininess);

	int light_col = glGetUniformLocation(shader->program, "light_color");
	glUniform3fv(light_col, 1, glm::value_ptr(light_color));

	glUniform1i(glGetUniformLocation(shader->program, "toon"), (int)toon);

	if (!texture1 && !texture2) {
		int no_l = 0;
		glUniform1i(glGetUniformLocation(shader->program, "no_layers"), no_l);
	}
	else {
		glUniform1i(glGetUniformLocation(shader->program, "no_layers"), no_layers);
	}
	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture1)
	{
		// TODO : activate texture location 0
		glActiveTexture(GL_TEXTURE0);
		//TODO : Bind the texture1 ID
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		//TODO : Send texture uniform value
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}

	if (texture2)
	{
		//TODO : activate texture location 1
		glActiveTexture(GL_TEXTURE1);
		//TODO : Bind the texture2 ID
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		//TODO : Send texture uniform value
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	}
}

void Tema3::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F)
	{
		toon = !toon;
	}
	if (key == GLFW_KEY_G)
	{
		outline = !outline;
	}
	if (key == GLFW_KEY_UP)
	{
		no_layers++;
	}
	if (key == GLFW_KEY_DOWN)
	{
		no_layers--;
	}
}

void Tema3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}
