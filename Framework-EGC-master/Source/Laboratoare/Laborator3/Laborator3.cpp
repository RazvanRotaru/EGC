#include "Laborator3.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Laborator3::Laborator3()
{
}

Laborator3::~Laborator3()
{
	delete[] asteroidPoint;
}

void Laborator3::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;
	
	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;
	
	// initialize angularStep
	angularStep = 0;

	moveX = 0;
	moveY = 0;

/*
	Create platforms
*/

	platformNR++;
	platformXcoords[1] = 700;
	platformYcoords[1] = 150;
	platformH[1] = 50;
	platformW[1] = 400;
	platformReflect[1] = false;

	Mesh* platform1 = Object2D::CreatePlatform("platform1", corner + glm::vec3(platformXcoords[1], platformYcoords[1], 0), platformW[1], platformH[1], platformRotation[1], glm::vec3(0, 1, 0.5), platformReflect[1]);
	AddMeshToList(platform1);

	platformNR++;
	platformXcoords[2] = 150;
	platformYcoords[2] = 600;
	platformH[2] = 50;
	platformW[2] = 400;
	platformReflect[2] = true;
	platformRotation[2] = M_PI / 6;

	Mesh* platform2 = Object2D::CreatePlatform("platform2", corner + glm::vec3(platformXcoords[2], platformYcoords[2], 0), platformW[2], platformH[2], platformRotation[2], glm::vec3(1, 0, 0), platformReflect[2]);
	AddMeshToList(platform2);

	platformNR++;
	platformXcoords[3] = 10;
	platformYcoords[3] = 150;
	platformH[3] = 400;
	platformW[3] = 50;
	platformReflect[3] = false;

	Mesh* platform3 = Object2D::CreatePlatform("platform3", corner + glm::vec3(platformXcoords[3], platformYcoords[3], 0), platformW[3], platformH[3], platformRotation[3], glm::vec3(0, 1, 0.5), platformReflect[3]);
	AddMeshToList(platform3);


	platformNR++;
	platformXcoords[4] = 0;
	platformYcoords[4] = -30;
	platformH[4] = 50;
	platformW[4] = resolution.x;
	platformReflect[4] = true;

	Mesh* platform4 = Object2D::CreatePlatform("platform4", corner + glm::vec3(platformXcoords[4], platformYcoords[4], 0), platformW[4], platformH[4], platformRotation[4], glm::vec3(1, 0, 0), platformReflect[4]);
	AddMeshToList(platform4);


	platformNR++;
	platformXcoords[5] = 0;
	platformYcoords[5] = resolution.y-20;
	platformH[5] = 50;
	platformW[5] = resolution.x;
	platformReflect[5] = true;

	Mesh* platform5 = Object2D::CreatePlatform("platform5", corner + glm::vec3(platformXcoords[5], platformYcoords[5], 0), platformW[5], platformH[5], platformRotation[5], glm::vec3(1, 0, 0), platformReflect[5]);
	AddMeshToList(platform5);


	platformNR++;
	platformXcoords[6] = -30;
	platformYcoords[6] = 0;
	platformH[6] = resolution.y;
	platformW[6] = 50;
	platformReflect[6] = true;

	Mesh* platform6 = Object2D::CreatePlatform("platform6", corner + glm::vec3(platformXcoords[6], platformYcoords[6], 0), platformW[6], platformH[6], platformRotation[6], glm::vec3(1, 0, 0), platformReflect[6]);
	AddMeshToList(platform6);


	platformNR++;
	platformXcoords[7] = resolution.x -20;
	platformYcoords[7] = 0;
	platformH[7] = resolution.y;
	platformW[7] = 50;
	platformReflect[7] = true;

	Mesh* platform7 = Object2D::CreatePlatform("platform7", corner + glm::vec3(platformXcoords[7], platformYcoords[7], 0), platformW[7], platformH[7], platformRotation[7], glm::vec3(1, 0, 0), platformReflect[7]);
	AddMeshToList(platform7);

	platformNR++;
	platformXcoords[8] = resolution.x - 100;
	platformYcoords[8] = resolution.y - 300;
	platformH[8] = 200;
	platformW[8] = 50;
	finalPlatform[8] = true;

	Mesh* platform8 = Object2D::CreatePlatform("platform8", corner + glm::vec3(platformXcoords[8], platformYcoords[8], 0), platformW[8], platformH[8], platformRotation[8], glm::vec3(1, 0, 1), platformReflect[8]);
	AddMeshToList(platform8);

/*
	Create astronaut
*/

	offX = startX = 150;
	prevX = 100;
	offY = startY = 170;
	prevY = 170;
	l = 90;
	A = make_pair(startX, startY + 2 * l / 3), B = make_pair(startX + l * sqrt(3) / 3, startY - l / 3), C = make_pair(startX - l*sqrt(3) / 3, startY - l / 3);
	Mesh* triangle1 = Object2D::CreateTriangle("triangle1", corner + glm::vec3(startX, startY, 0), l, glm::vec3(1, 1, 1), true);
	AddMeshToList(triangle1);

/*
	Create asteroids
*/
	asteroidPoint = new std::vector<std::pair <float, float> >[5];
	asteroidNR++;
	asteroidX[asteroidNR] = 720;
	asteroidY[asteroidNR] = 500;
	asteroidR[asteroidNR] = 50;
	
	asteroidPoint[asteroidNR].push_back(std::make_pair(asteroidX[asteroidNR] - asteroidR[asteroidNR], asteroidY[asteroidNR]));
	asteroidPoint[asteroidNR].push_back(std::make_pair(asteroidX[asteroidNR] - asteroidR[asteroidNR] * sqrt(2) / 2.0, asteroidY[asteroidNR] - asteroidR[asteroidNR] * sqrt(2) / 2.0));
	asteroidPoint[asteroidNR].push_back(std::make_pair(asteroidX[asteroidNR], asteroidY[asteroidNR] - asteroidR[asteroidNR]));
	asteroidPoint[asteroidNR].push_back(std::make_pair(asteroidX[asteroidNR] + asteroidR[asteroidNR] * sqrt(2) / 2, asteroidY[asteroidNR] - asteroidR[asteroidNR] * sqrt(2) / 2));
	asteroidPoint[asteroidNR].push_back(std::make_pair(asteroidX[asteroidNR] + asteroidR[asteroidNR], asteroidY[asteroidNR]));
	asteroidPoint[asteroidNR].push_back(std::make_pair(asteroidX[asteroidNR] + asteroidR[asteroidNR] * sqrt(2) / 2, asteroidY[asteroidNR] + asteroidR[asteroidNR] * sqrt(2) / 2));
	asteroidPoint[asteroidNR].push_back(std::make_pair(asteroidX[asteroidNR], asteroidY[asteroidNR] + asteroidR[asteroidNR]));
	asteroidPoint[asteroidNR].push_back(std::make_pair(asteroidX[asteroidNR] - asteroidR[asteroidNR] * sqrt(2) / 2, asteroidY[asteroidNR] + asteroidR[asteroidNR] * sqrt(2) / 2));

	cout << asteroidPoint[asteroidNR].at(0).first << endl;
	Mesh* asteroid1 = Object2D::CreateAsteroid("asteroid1", corner + glm::vec3(asteroidX[asteroidNR], asteroidY[asteroidNR], 0), asteroidR[asteroidNR], glm::vec3(0.5, 0.5, 0), true);
	AddMeshToList(asteroid1);
}

void Laborator3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator3::Update(float deltaTimeSeconds)
{

	if (!GameOver) {
		moveX += translateX * deltaTimeSeconds *0.5;
		moveY += translateY * deltaTimeSeconds *0.5;

		if (glm::distance(glm::vec3(asteroidMove[1], 0), glm::vec3(0, 0, 0)) < 200) {
			asteroidMove[1] += glm::vec2(deltaTimeSeconds * 10, deltaTimeSeconds * 10);
		}
		else {
			asteroidMove[1] -= glm::vec2(deltaTimeSeconds * 10, deltaTimeSeconds * 10);
		}
		asteroidAngl[1] -= deltaTimeSeconds;

		if (alpha * lsgn > 0 && stopped) {

			cout << alpha << endl;
			angularStep += alpha * deltaTimeSeconds;
			lsgn = alpha;
			alpha -= alpha * deltaTimeSeconds;

		}

		if (angularSteptoBe != -1 && stopped) {
			alpha = angularSteptoBe - angularStep;
			lsgn = alpha;
			angularSteptoBe = -1;
		}

		if (translateX != 0 && translateY != 0) {
			stopped = false;
		}


		// Folosesc suma Minkovsky pentru a determina spatiile Gausiene in care se afla platformele si triunghiul

		for (int i = 1; i <= platformNR; i++) {

			float w, h, dx, dy;

			w = 0.5 * (2 * l / 3 + platformW[i]);
			h = 0.5 * (2 * l / 3 + platformH[i]);

			if (platformRotation[i]) {

				float X1 = (Transform2D::Translate(platformXcoords[i], platformYcoords[i]) * Transform2D::Rotate(-platformRotation[i]) * Transform2D::Translate(-platformXcoords[i], -platformYcoords[i]) * glm::vec3(startX + moveX, startY + moveY, 0)).x;
				float Y1 = (Transform2D::Translate(platformXcoords[i], platformYcoords[i]) * Transform2D::Rotate(-platformRotation[i]) * Transform2D::Translate(-platformXcoords[i], -platformYcoords[i]) * glm::vec3(startX + moveX, startY + moveY, 0)).y;

				float X2 = (Transform2D::Translate(platformXcoords[i], platformYcoords[i]) * Transform2D::Rotate(-platformRotation[i]) * Transform2D::Translate(-platformXcoords[i], -platformYcoords[i]) * glm::vec3((platformXcoords[i] + platformW[i] / 2), (platformYcoords[i] + platformH[i] / 2), 0)).x;
				float Y2 = (Transform2D::Translate(platformXcoords[i], platformYcoords[i]) * Transform2D::Rotate(-platformRotation[i]) * Transform2D::Translate(-platformXcoords[i], -platformYcoords[i]) * glm::vec3((platformXcoords[i] + platformW[i] / 2), (platformYcoords[i] + platformH[i] / 2), 0)).y;

				dx = X1 - X2;
				dy = Y1 - Y2;
				//cout << endl;
			}
			else {
				dx = startX + moveX - (platformXcoords[i] + platformW[i] / 2);
				dy = startY + moveY - (platformYcoords[i] + platformH[i] / 2);
			}

			if (abs(dx) <= w && abs(dy) <= h && lastHit != i)
			{
				/* collision! */

				if (finalPlatform[i]) {
					GameOver = true;
					break;
				}
				lastAsteroidHit = 0;
				lastHit = i;
				glm::vec3 reflect, direction, normal;
				direction = glm::vec3(translateX, translateY, 0);
				float wy = w * dy;
				float hx = h * dx;

				if (wy >= hx) {
					if (wy >= -hx) {
						/* collision at the top */
						if (platformReflect[i]) {
							normal = glm::vec3(glm::normalize(Transform2D::Rotate(platformRotation[i])*glm::vec3(0, platformW[i], 0)));
						}
						else if (!stopped) {
							angularSteptoBe = 0;
						}
					}
					else {
						/* on the left */
						if (platformReflect[i]) {
							normal = glm::vec3(glm::normalize(Transform2D::Rotate(platformRotation[i])*glm::vec3(platformH[i], 0, 0)));
						}
						else if (!stopped) {
							angularSteptoBe = M_PI / 2;
						}
					}
				}
				else {
					if (wy >= -hx) {
						/* on the right */
						if (platformReflect[i]) {
							normal = glm::vec3(glm::normalize(Transform2D::Rotate(platformRotation[i])*glm::vec3(platformH[i], 0, 0)));
						}
						else if (!stopped) {
							angularSteptoBe = -M_PI / 2;
						}
					}
					else {
						/* at the bottom */
						if (platformReflect[i]) {
							normal = glm::vec3(glm::normalize(Transform2D::Rotate(platformRotation[i])*glm::vec3(0, platformW[i], 0)));
						}
						else if (!stopped) {
							angularSteptoBe = M_PI;
						}
					}
				}

				if (platformReflect[i]) {
					reflect = glm::reflect(direction, normal);
					translateX = reflect.x;
					translateY = reflect.y;
					angularStep = atan2(translateY, translateX) - M_PI / 2;
				}
				else {
					translateX = 0;
					translateY = 0;
					offX = startX + moveX;
					offY = startY + moveY;
					stopped = true;
				}

			}
		}

		for (int i = 1; i <= asteroidNR; i++) {

			float intersectX = startX + moveX + 2 * l * cos(angularStep + M_PI / 2) / 3;
			float intersectY = startY + moveY + 2 * l * sin(angularStep + M_PI / 2) / 3;

			float dist = glm::distance(glm::vec3(intersectX, intersectY, 0), glm::vec3(asteroidX[i] + asteroidMove[i].x, asteroidY[i] + asteroidMove[i].y, 0));

			if (dist <= asteroidR[i] && !asteroidDistroyed[i]) {

				asteroidDistroyed[i] = true;
				lastHit = 0;
				glm::vec3 reflect, direction, normal;
				direction = glm::vec3(translateX, translateY, 0);
				normal = glm::vec3(glm::normalize(glm::vec3(asteroidX[i] + asteroidMove[i].x - intersectX, asteroidY[i] + asteroidMove[i].y - intersectY, 0)));
				reflect = glm::reflect(direction, normal);

				translateX = reflect.x;
				translateY = reflect.y;
				angularStep = atan2(translateY, translateX) - M_PI / 2;
			}
		}
	}
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(moveX, moveY);
	modelMatrix *= Transform2D::Translate(startX, startY);	
	modelMatrix *= Transform2D::Rotate(angularStep);
	modelMatrix *= Transform2D::Translate(-startX, -startY);
	RenderMesh2D(meshes["triangle1"], shaders["VertexColor"], modelMatrix);
	
	for (int i = 1; i <= asteroidNR; i++) {
	
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(asteroidMove[i].x, asteroidMove[i].y);
		modelMatrix *= Transform2D::Translate(asteroidX[i], asteroidY[i]);
		modelMatrix *= Transform2D::Rotate(asteroidAngl[i]);
		modelMatrix *= Transform2D::Translate(-asteroidX[i], -asteroidY[i]);
		if (!asteroidDistroyed[i])
			RenderMesh2D(meshes["asteroid" + to_string(i)], shaders["VertexColor"], modelMatrix);
	}
	
	for (int i = 1; i <= platformNR; i++) {
		modelMatrix = glm::mat3(1);
		if (platformRotation[i]) {
			modelMatrix *= Transform2D::Translate(platformXcoords[i] + platformW[i] / 2, platformYcoords[i] + platformH[i] / 2);
			modelMatrix *= Transform2D::Rotate(platformRotation[i]);
			modelMatrix *= Transform2D::Translate(-platformXcoords[i] - platformW[i] / 2, -platformYcoords[i] - platformH[i] / 2);
		}
		RenderMesh2D(meshes["platform"+to_string(i)], shaders["VertexColor"], modelMatrix);
	}
}

void Laborator3::FrameEnd()
{

}

void Laborator3::OnInputUpdate(float deltaTime, int mods)
{
	
}

void Laborator3::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Laborator3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

}

void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{	
	// add mouse button press event
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && stopped) {
		translateX = mouseX - offX;
		translateY = window->GetResolution().y - mouseY - offY;
		angularStep = atan2(translateY, translateX) - M_PI/2;
	}
}

void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator3::OnWindowResize(int width, int height)
{
}
