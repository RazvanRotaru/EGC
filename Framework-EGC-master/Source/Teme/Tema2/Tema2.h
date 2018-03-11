#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include "Obiecte\Strada.h"
#include "Obiecte\Masina.h"
#include "Obiecte\Colt.h"
#include "Obiecte\Camera.h"
#include "Transform3D.h"

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;
	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix);
	void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix);
	Camera* getCamera();
	glm::mat4 getProjectionMatrix();

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void isGameOver();
	void VerifyColision();
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	std::vector<Strada*>s;
	Masina *bmw_seria3_din_2007;
	Camera* camera;
	Mesh * iarba;
	Mesh * cer;
	GLenum polygonMode;
	glm::mat4 projectionMatrix;
	int camType;
	bool gameOver, ready;
	float translateX, translateY, translateZ;
	int wheel, points, colisions;
};
