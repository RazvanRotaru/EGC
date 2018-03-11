#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	glm::mat3 modelMatrix;
	float translateX, translateY;
	float scaleX, scaleY;
	float offX, offY, moveX, moveY, startX, startY, l;
	std::pair<float, float> A, B, C;
	float prevX, prevY;
	float asteroidX[10], asteroidY[10], asteroidR[10];
	float asteroidAngl[10], asteroidScale[10];
	bool asteroidDistroyed[10], asteroidLimit[10];
	bool finalPlatform[10];
	bool GameOver;
	int asteroidNR = 0;
	glm::vec2 asteroidMove[10];
	float platformXcoords[10], platformYcoords[10], platformH[10], platformW[10], platformRotation[10];
	bool platformReflect[10], edge[10];
	bool stopped = true;
	float angularStep, angularSteptoBe = -1, alpha, lsgn;
	float ex = 0;
	bool platformHit = false, asteroidHit = false;
	int platformNR = 0, lastHit = 0, lastAsteroidHit = 0;
};
