#include <Windows.h>
#include <iostream>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "inputManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <stack>

using namespace glm;

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 800;

Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");

static bool isWallRotationOn = false;
static int wallIndex = -1;

// Degrees that the wall traveld during one key stroke 
static float rotationAngleTraveld = 0.0f;
static float rotationAngle = 90.0f;
static float rotationArg = 0.5f;

// Degrees that the wall traveld from the last time it was aligned
static float wallTravelAngle = 0.0f;
static bool rotationChangedFlag = false;



vec3 pos = vec3(0, 0, -15);
vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
vec3 cubeUp = up;
vec3 cubeForward = forward;


mat4 transformations[27];
mat4 cubeRotation[27];

// Holds a stack of the middle cube in a wall, between roatation stgages
std::stack<mat4> middleCubeTemp;

struct WallsStructure
{
	int rightWall[9];
	int leftWall[9];
	int frontWall[9];
	int backWall[9];
	int upperWall[9];
	int lowerWall[9];

} wallsStructure;


void initializeMatrices() {

	transformations[0] = glm::mat4(1); //(0,0,0) 
	transformations[1] = glm::translate(mat4(1), vec3(2.04, 0, 0)); //(1,0,0)
	transformations[2] = glm::translate(mat4(1), vec3(-2.04, 0, 0));//(-1,0,0)
	transformations[3] = glm::translate(mat4(1), vec3(0, 2.04, 0));//(0,1,0)
	transformations[4] = glm::translate(mat4(1), vec3(2.04, 2.04, 0));//(1,1,0)
	transformations[5] = glm::translate(mat4(1), vec3(-2.04, 2.04, 0));//(-1,1,0)
	transformations[6] = glm::translate(mat4(1), vec3(0, -2.04, 0));//(0,-1,0)
	transformations[7] = glm::translate(mat4(1), vec3(2.04, -2.04, 0));//(1,-1,0)
	transformations[8] = glm::translate(mat4(1), vec3(-2.04, -2.04, 0));//(-1,-1,0)
	transformations[9] = glm::translate(mat4(1), vec3(0, 0, -2.04));//(0,0,-1)
	transformations[10] = glm::translate(mat4(1), vec3(2.04, 0, -2.04));//(1,0,-1)
	transformations[11] = glm::translate(mat4(1), vec3(-2.04, 0, -2.04));//(-1,0,-1)
	transformations[12] = glm::translate(mat4(1), vec3(0, 2.04, -2.04));//(0,1,-1)
	transformations[13] = glm::translate(mat4(1), vec3(2.04, 2.04, -2.04));//(1,1,-1)
	transformations[14] = glm::translate(mat4(1), vec3(-2.04, 2.04, -2.04));//(-1,1,-1)
	transformations[15] = glm::translate(mat4(1), vec3(0, -2.04, -2.04));//(0,-1,-1)
	transformations[16] = glm::translate(mat4(1), vec3(2.04, -2.04, -2.04));//(1,-1,-1)
	transformations[17] = glm::translate(mat4(1), vec3(-2.04, -2.04, -2.04));//(-1,-1,-1)
	transformations[18] = glm::translate(mat4(1), vec3(0, 0, 2.04));//(0,0,1)
	transformations[19] = glm::translate(mat4(1), vec3(2.04, 0, 2.04));//(1,0,1)
	transformations[20] = glm::translate(mat4(1), vec3(-2.04, 0, 2.04));//(-1,0,1)
	transformations[21] = glm::translate(mat4(1), vec3(0, 2.04, 2.04));//(0,1,1)
	transformations[22] = glm::translate(mat4(1), vec3(2.04, 2.04, 2.04));//(1,1,1)
	transformations[23] = glm::translate(mat4(1), vec3(-2.04, 2.04, 2.04));//(-1,1,1)
	transformations[24] = glm::translate(mat4(1), vec3(0, -2.04, 2.04));//(0,-1,1)
	transformations[25] = glm::translate(mat4(1), vec3(2.04, -2.04, 2.04));//(1,-1,1)
	transformations[26] = glm::translate(mat4(1), vec3(-2.04, -2.04, 2.04));//(-1,-1,1)


}

void initializeWallStructure() {

	int j;

	for (int i = 0; i < 9; i++) {

		j = i / 3;

		wallsStructure.rightWall[i] = 2 + 3 * i;
		wallsStructure.leftWall[i] = 1 + 3 * i;
		wallsStructure.frontWall[i] = 9 + i;
		wallsStructure.backWall[i] = 18 + i;
		wallsStructure.upperWall[i] = 3 + (i % 3) + (9 * j);
		wallsStructure.lowerWall[i] = 6 + (i % 3) + (9 * j);

	}

}


void updateWallStructureClockwise() {

	switch (wallIndex) {
	case 0: {

		wallsStructure.frontWall[5] = wallsStructure.rightWall[5];
		wallsStructure.frontWall[8] = wallsStructure.rightWall[8];
		wallsStructure.frontWall[2] = wallsStructure.rightWall[2];
		wallsStructure.backWall[5] = wallsStructure.rightWall[4];
		wallsStructure.backWall[8] = wallsStructure.rightWall[7];
		wallsStructure.backWall[2] = wallsStructure.rightWall[1];
		wallsStructure.upperWall[5] = wallsStructure.rightWall[5];
		wallsStructure.upperWall[8] = wallsStructure.rightWall[4];
		wallsStructure.upperWall[2] = wallsStructure.rightWall[3];
		wallsStructure.lowerWall[5] = wallsStructure.rightWall[8];
		wallsStructure.lowerWall[8] = wallsStructure.rightWall[7];
		wallsStructure.lowerWall[2] = wallsStructure.rightWall[6];


		int temp[9];
		for (int i = 0; i < 9; i++) {
			temp[i] = wallsStructure.rightWall[i];

		}

		wallsStructure.rightWall[1] = temp[3];
		wallsStructure.rightWall[2] = temp[6];
		wallsStructure.rightWall[3] = temp[2];
		wallsStructure.rightWall[4] = temp[5];
		wallsStructure.rightWall[5] = temp[8];
		wallsStructure.rightWall[6] = temp[1];
		wallsStructure.rightWall[7] = temp[4];
		wallsStructure.rightWall[8] = temp[7];

	}
			break;
	case 1:
	{
		wallsStructure.frontWall[7] = wallsStructure.leftWall[8];
		wallsStructure.frontWall[1] = wallsStructure.leftWall[2];
		wallsStructure.frontWall[4] = wallsStructure.leftWall[5];
		wallsStructure.backWall[4] = wallsStructure.leftWall[4];
		wallsStructure.backWall[1] = wallsStructure.leftWall[1];
		wallsStructure.backWall[7] = wallsStructure.leftWall[7];
		wallsStructure.upperWall[4] = wallsStructure.leftWall[5];
		wallsStructure.upperWall[1] = wallsStructure.leftWall[3];
		wallsStructure.upperWall[7] = wallsStructure.leftWall[4];
		wallsStructure.lowerWall[4] = wallsStructure.leftWall[8];
		wallsStructure.lowerWall[1] = wallsStructure.leftWall[6];
		wallsStructure.lowerWall[7] = wallsStructure.leftWall[7];

		int temp[9];
		for (int i = 0; i < 9; i++) {
			temp[i] = wallsStructure.leftWall[i];

		}

		wallsStructure.leftWall[1] = temp[3];
		wallsStructure.leftWall[2] = temp[6];
		wallsStructure.leftWall[3] = temp[2];
		wallsStructure.leftWall[4] = temp[5];
		wallsStructure.leftWall[5] = temp[8];
		wallsStructure.leftWall[6] = temp[1];
		wallsStructure.leftWall[7] = temp[4];
		wallsStructure.leftWall[8] = temp[7];


	}
	break;
	case 2: {

		wallsStructure.rightWall[4] = wallsStructure.upperWall[8];
		wallsStructure.rightWall[1] = wallsStructure.upperWall[6];
		wallsStructure.rightWall[7] = wallsStructure.upperWall[7];
		wallsStructure.leftWall[4] = wallsStructure.upperWall[5];
		wallsStructure.leftWall[1] = wallsStructure.upperWall[3];
		wallsStructure.leftWall[7] = wallsStructure.upperWall[4];
		wallsStructure.frontWall[4] = wallsStructure.upperWall[5];
		wallsStructure.frontWall[3] = wallsStructure.upperWall[2];
		wallsStructure.frontWall[5] = wallsStructure.upperWall[8];
		wallsStructure.backWall[4] = wallsStructure.upperWall[4];
		wallsStructure.backWall[3] = wallsStructure.upperWall[1];
		wallsStructure.backWall[5] = wallsStructure.upperWall[7];

		int temp[9];
		for (int i = 0; i < 9; i++) {
			temp[i] = wallsStructure.upperWall[i];

		}

		wallsStructure.upperWall[1] = temp[3];
		wallsStructure.upperWall[2] = temp[6];
		wallsStructure.upperWall[3] = temp[2];
		wallsStructure.upperWall[4] = temp[5];
		wallsStructure.upperWall[5] = temp[8];
		wallsStructure.upperWall[6] = temp[1];
		wallsStructure.upperWall[7] = temp[4];
		wallsStructure.upperWall[8] = temp[7];



	}
			break;
	case 3: {

		wallsStructure.rightWall[8] = wallsStructure.lowerWall[7];
		wallsStructure.rightWall[2] = wallsStructure.lowerWall[6];
		wallsStructure.rightWall[5] = wallsStructure.lowerWall[8];
		wallsStructure.leftWall[5] = wallsStructure.lowerWall[5];
		wallsStructure.leftWall[2] = wallsStructure.lowerWall[3];
		wallsStructure.leftWall[8] = wallsStructure.lowerWall[4];
		wallsStructure.frontWall[8] = wallsStructure.lowerWall[8];
		wallsStructure.frontWall[6] = wallsStructure.lowerWall[2];
		wallsStructure.frontWall[7] = wallsStructure.lowerWall[5];
		wallsStructure.backWall[7] = wallsStructure.lowerWall[4];
		wallsStructure.backWall[6] = wallsStructure.lowerWall[1];
		wallsStructure.backWall[8] = wallsStructure.lowerWall[7];

		int temp[9];
		for (int i = 0; i < 9; i++) {
			temp[i] = wallsStructure.lowerWall[i];

		}

		wallsStructure.lowerWall[1] = temp[3];
		wallsStructure.lowerWall[2] = temp[6];
		wallsStructure.lowerWall[3] = temp[2];
		wallsStructure.lowerWall[4] = temp[5];
		wallsStructure.lowerWall[5] = temp[8];
		wallsStructure.lowerWall[6] = temp[1];
		wallsStructure.lowerWall[7] = temp[4];
		wallsStructure.lowerWall[8] = temp[7];


	}
			break;
	case 4: {

		wallsStructure.rightWall[7] = wallsStructure.backWall[4];
		wallsStructure.rightWall[6] = wallsStructure.backWall[3];
		wallsStructure.rightWall[8] = wallsStructure.backWall[5];
		wallsStructure.leftWall[8] = wallsStructure.backWall[8];
		wallsStructure.leftWall[6] = wallsStructure.backWall[6];
		wallsStructure.leftWall[7] = wallsStructure.backWall[7];
		wallsStructure.upperWall[7] = wallsStructure.backWall[7];
		wallsStructure.upperWall[6] = wallsStructure.backWall[1];
		wallsStructure.upperWall[8] = wallsStructure.backWall[4];
		wallsStructure.lowerWall[8] = wallsStructure.backWall[5];
		wallsStructure.lowerWall[6] = wallsStructure.backWall[2];
		wallsStructure.lowerWall[7] = wallsStructure.backWall[8];

		int temp[9];
		for (int i = 0; i < 9; i++) {
			temp[i] = wallsStructure.backWall[i];
		}

		wallsStructure.backWall[1] = temp[6];
		wallsStructure.backWall[2] = temp[3];
		wallsStructure.backWall[3] = temp[1];
		wallsStructure.backWall[4] = temp[7];
		wallsStructure.backWall[5] = temp[4];
		wallsStructure.backWall[6] = temp[2];
		wallsStructure.backWall[7] = temp[8];
		wallsStructure.backWall[8] = temp[5];

	}
			break;
	case 5: {

		wallsStructure.rightWall[4] = wallsStructure.frontWall[4];
		wallsStructure.rightWall[3] = wallsStructure.frontWall[3];
		wallsStructure.rightWall[5] = wallsStructure.frontWall[5];
		wallsStructure.leftWall[5] = wallsStructure.frontWall[8];
		wallsStructure.leftWall[3] = wallsStructure.frontWall[6];
		wallsStructure.leftWall[4] = wallsStructure.frontWall[7];
		wallsStructure.upperWall[4] = wallsStructure.frontWall[7];
		wallsStructure.upperWall[3] = wallsStructure.frontWall[1];
		wallsStructure.upperWall[5] = wallsStructure.frontWall[4];
		wallsStructure.lowerWall[5] = wallsStructure.frontWall[5];
		wallsStructure.lowerWall[3] = wallsStructure.frontWall[2];
		wallsStructure.lowerWall[4] = wallsStructure.frontWall[8];

		int temp[9];
		for (int i = 0; i < 9; i++) {
			temp[i] = wallsStructure.frontWall[i];
		}

		wallsStructure.frontWall[1] = temp[6];
		wallsStructure.frontWall[2] = temp[3];
		wallsStructure.frontWall[3] = temp[1];
		wallsStructure.frontWall[4] = temp[7];
		wallsStructure.frontWall[5] = temp[4];
		wallsStructure.frontWall[6] = temp[2];
		wallsStructure.frontWall[7] = temp[8];
		wallsStructure.frontWall[8] = temp[5];

	}
			break;

	}
}

void updateWallStructureCounterClockwise() {

	switch (wallIndex) {
	case 0: {

		wallsStructure.frontWall[5] = wallsStructure.rightWall[7];
		wallsStructure.frontWall[8] = wallsStructure.rightWall[4];
		wallsStructure.frontWall[2] = wallsStructure.rightWall[1];
		wallsStructure.backWall[5] = wallsStructure.rightWall[8];
		wallsStructure.backWall[8] = wallsStructure.rightWall[5];
		wallsStructure.backWall[2] = wallsStructure.rightWall[2];
		wallsStructure.upperWall[5] = wallsStructure.rightWall[7];
		wallsStructure.upperWall[8] = wallsStructure.rightWall[8];
		wallsStructure.upperWall[2] = wallsStructure.rightWall[6];
		wallsStructure.lowerWall[5] = wallsStructure.rightWall[4];
		wallsStructure.lowerWall[8] = wallsStructure.rightWall[5];
		wallsStructure.lowerWall[2] = wallsStructure.rightWall[3];


		int temp[9];
		for (int i = 0; i < 9; i++) {
			temp[i] = wallsStructure.rightWall[i];

		}

		wallsStructure.rightWall[1] = temp[6];
		wallsStructure.rightWall[2] = temp[3];
		wallsStructure.rightWall[3] = temp[1];
		wallsStructure.rightWall[4] = temp[7];
		wallsStructure.rightWall[5] = temp[4];
		wallsStructure.rightWall[6] = temp[2];
		wallsStructure.rightWall[7] = temp[8];
		wallsStructure.rightWall[8] = temp[5];

	}
			break;
	case 1:
	{
		wallsStructure.frontWall[7] = wallsStructure.leftWall[4];
		wallsStructure.frontWall[1] = wallsStructure.leftWall[1];
		wallsStructure.frontWall[4] = wallsStructure.leftWall[7];
		wallsStructure.backWall[4] = wallsStructure.leftWall[8];
		wallsStructure.backWall[1] = wallsStructure.leftWall[2];
		wallsStructure.backWall[7] = wallsStructure.leftWall[5];
		wallsStructure.upperWall[4] = wallsStructure.leftWall[7];
		wallsStructure.upperWall[1] = wallsStructure.leftWall[6];
		wallsStructure.upperWall[7] = wallsStructure.leftWall[8];
		wallsStructure.lowerWall[4] = wallsStructure.leftWall[4];
		wallsStructure.lowerWall[1] = wallsStructure.leftWall[3];
		wallsStructure.lowerWall[7] = wallsStructure.leftWall[5];

		int temp[9];
		for (int i = 0; i < 9; i++) {
			temp[i] = wallsStructure.leftWall[i];

		}

		wallsStructure.leftWall[1] = temp[6];
		wallsStructure.leftWall[2] = temp[3];
		wallsStructure.leftWall[3] = temp[1];
		wallsStructure.leftWall[4] = temp[7];
		wallsStructure.leftWall[5] = temp[4];
		wallsStructure.leftWall[6] = temp[2];
		wallsStructure.leftWall[7] = temp[8];
		wallsStructure.leftWall[8] = temp[5];


	}
	break;
	case 2: {

		wallsStructure.rightWall[4] = wallsStructure.upperWall[4];
		wallsStructure.rightWall[1] = wallsStructure.upperWall[3];
		wallsStructure.rightWall[7] = wallsStructure.upperWall[5];
		wallsStructure.leftWall[4] = wallsStructure.upperWall[7];
		wallsStructure.leftWall[1] = wallsStructure.upperWall[6];
		wallsStructure.leftWall[7] = wallsStructure.upperWall[8];
		wallsStructure.frontWall[4] = wallsStructure.upperWall[7];
		wallsStructure.frontWall[3] = wallsStructure.upperWall[1];
		wallsStructure.frontWall[5] = wallsStructure.upperWall[4];
		wallsStructure.backWall[4] = wallsStructure.upperWall[8];
		wallsStructure.backWall[3] = wallsStructure.upperWall[2];
		wallsStructure.backWall[5] = wallsStructure.upperWall[5];

		int temp[9];
		for (int i = 0; i < 9; i++) {
			temp[i] = wallsStructure.upperWall[i];

		}

		wallsStructure.upperWall[1] = temp[6];
		wallsStructure.upperWall[2] = temp[3];
		wallsStructure.upperWall[3] = temp[1];
		wallsStructure.upperWall[4] = temp[7];
		wallsStructure.upperWall[5] = temp[4];
		wallsStructure.upperWall[6] = temp[2];
		wallsStructure.upperWall[7] = temp[8];
		wallsStructure.upperWall[8] = temp[5];



	}
			break;
	case 3: {

		wallsStructure.rightWall[8] = wallsStructure.lowerWall[5];
		wallsStructure.rightWall[2] = wallsStructure.lowerWall[3];
		wallsStructure.rightWall[5] = wallsStructure.lowerWall[4];
		wallsStructure.leftWall[5] = wallsStructure.lowerWall[7];
		wallsStructure.leftWall[2] = wallsStructure.lowerWall[6];
		wallsStructure.leftWall[8] = wallsStructure.lowerWall[8];
		wallsStructure.frontWall[8] = wallsStructure.lowerWall[4];
		wallsStructure.frontWall[6] = wallsStructure.lowerWall[1];
		wallsStructure.frontWall[7] = wallsStructure.lowerWall[7];
		wallsStructure.backWall[7] = wallsStructure.lowerWall[8];
		wallsStructure.backWall[6] = wallsStructure.lowerWall[2];
		wallsStructure.backWall[8] = wallsStructure.lowerWall[5];

		int temp[9];
		for (int i = 0; i < 9; i++) {
			temp[i] = wallsStructure.lowerWall[i];

		}

		wallsStructure.lowerWall[1] = temp[6];
		wallsStructure.lowerWall[2] = temp[3];
		wallsStructure.lowerWall[3] = temp[1];
		wallsStructure.lowerWall[4] = temp[7];
		wallsStructure.lowerWall[5] = temp[4];
		wallsStructure.lowerWall[6] = temp[2];
		wallsStructure.lowerWall[7] = temp[8];
		wallsStructure.lowerWall[8] = temp[5];


	}
			break;
	case 4: {

		wallsStructure.rightWall[7] = wallsStructure.backWall[8];
		wallsStructure.rightWall[6] = wallsStructure.backWall[6];
		wallsStructure.rightWall[8] = wallsStructure.backWall[7];
		wallsStructure.leftWall[8] = wallsStructure.backWall[4];
		wallsStructure.leftWall[6] = wallsStructure.backWall[3];
		wallsStructure.leftWall[7] = wallsStructure.backWall[5];
		wallsStructure.upperWall[7] = wallsStructure.backWall[5];
		wallsStructure.upperWall[6] = wallsStructure.backWall[2];
		wallsStructure.upperWall[8] = wallsStructure.backWall[8];
		wallsStructure.lowerWall[8] = wallsStructure.backWall[7];
		wallsStructure.lowerWall[6] = wallsStructure.backWall[1];
		wallsStructure.lowerWall[7] = wallsStructure.backWall[4];

		int temp[9];
		for (int i = 0; i < 9; i++) {
			temp[i] = wallsStructure.backWall[i];
		}

		wallsStructure.backWall[1] = temp[3];
		wallsStructure.backWall[2] = temp[6];
		wallsStructure.backWall[3] = temp[2];
		wallsStructure.backWall[4] = temp[5];
		wallsStructure.backWall[5] = temp[8];
		wallsStructure.backWall[6] = temp[1];
		wallsStructure.backWall[7] = temp[4];
		wallsStructure.backWall[8] = temp[7];

	}
			break;
	case 5: {

		wallsStructure.rightWall[4] = wallsStructure.frontWall[8];
		wallsStructure.rightWall[3] = wallsStructure.frontWall[6];
		wallsStructure.rightWall[5] = wallsStructure.frontWall[7];
		wallsStructure.leftWall[5] = wallsStructure.frontWall[4];
		wallsStructure.leftWall[3] = wallsStructure.frontWall[3];
		wallsStructure.leftWall[4] = wallsStructure.frontWall[5];
		wallsStructure.upperWall[4] = wallsStructure.frontWall[5];
		wallsStructure.upperWall[3] = wallsStructure.frontWall[2];
		wallsStructure.upperWall[5] = wallsStructure.frontWall[8];
		wallsStructure.lowerWall[5] = wallsStructure.frontWall[7];
		wallsStructure.lowerWall[3] = wallsStructure.frontWall[1];
		wallsStructure.lowerWall[4] = wallsStructure.frontWall[4];

		int temp[9];
		for (int i = 0; i < 9; i++) {
			temp[i] = wallsStructure.frontWall[i];
		}

		wallsStructure.frontWall[1] = temp[3];
		wallsStructure.frontWall[2] = temp[6];
		wallsStructure.frontWall[3] = temp[2];
		wallsStructure.frontWall[4] = temp[5];
		wallsStructure.frontWall[5] = temp[8];
		wallsStructure.frontWall[6] = temp[1];
		wallsStructure.frontWall[7] = temp[4];
		wallsStructure.frontWall[8] = temp[7];

	}
			break;

	}
}

/*In order to keep the rotation axii aligned correctly,
thid method rotates back the middle to how it was before
the wall rotation. should be invisible to the user.*/
void restoreMiddleCubeTransofrmation() {

	mat4 previous;
	while (!middleCubeTemp.empty()) {
		previous = middleCubeTemp.top();
		middleCubeTemp.pop();
	}

	switch (wallIndex) {
	case 0: {
		transformations[wallsStructure.rightWall[0]] = previous;
	}
			break;
	case 1: {
		transformations[wallsStructure.leftWall[0]] = previous;
	}
			break;
	case 2: {
		transformations[wallsStructure.upperWall[0]] = previous;
	}
			break;
	case 3: {
		transformations[wallsStructure.lowerWall[0]] = previous;
	}
			break;
	case 4: {
		transformations[wallsStructure.backWall[0]] = previous;
	}
			break;
	case 5: {
		transformations[wallsStructure.frontWall[0]] = previous;
	}
			break;
	}
}


void updateAllRotations(vec3 axis) {

	for (int i = 0; i < 27; i++) {
		cubeRotation[i] = glm::rotate(cubeRotation[i], 0.4f, axis);
	}

}

void InputManager::rotateAboutXAxis(bool direction) {

	glfwSetKeyCallback(display.m_window, InputManager::key_callback_release);

	if (isWallRotationOn)
		return;

	vec3 axis = vec3(glm::cross(up, forward));
	if (!direction)
		axis = -axis;
	updateAllRotations(axis);

}

void InputManager::rotateAboutYAxis(bool direction) {

	glfwSetKeyCallback(display.m_window, InputManager::key_callback_release);

	if (isWallRotationOn)
		return;

	vec3 axis = up;
	if (!direction)
		axis = -axis;
	updateAllRotations(axis);


}

void registerCallbackIfWallsNotAligned() {

	if (wallTravelAngle != 0) {
		switch (wallIndex)
		{
		case 0:
			glfwSetKeyCallback(display.m_window, InputManager::key_callback_R);
			break;
		case 1:
			glfwSetKeyCallback(display.m_window, InputManager::key_callback_L);

			break;
		case 2:
			glfwSetKeyCallback(display.m_window, InputManager::key_callback_U);

			break;
		case 3:
			glfwSetKeyCallback(display.m_window, InputManager::key_callback_D);

			break;
		case 4:
			glfwSetKeyCallback(display.m_window, InputManager::key_callback_B);

			break;
		case 5:
			glfwSetKeyCallback(display.m_window, InputManager::key_callback_F);

			break;

		default:
			break;
		}
	}

}

void InputManager::stopRotation() {

	for (int i = 0; i < 27; i++) {

		cubeRotation[i] = mat4(1);
	}

	if (middleCubeTemp.empty()) {
		cubeUp = vec3((transformations[wallsStructure.upperWall[0]] * vec4(cubeUp, 1))) - vec3((transformations[wallsStructure.lowerWall[0]] * vec4(cubeUp, 1)));
		cubeForward = vec3(transformations[wallsStructure.backWall[0]] * vec4(cubeForward, 1)) - vec3(transformations[wallsStructure.frontWall[0]] * vec4(cubeForward, 1));
	}

	//Reenable keyboard
	glfwSetKeyCallback(display.m_window, InputManager::key_callback);
}

void stopWallRotation() {


	isWallRotationOn = false;
	rotationAngleTraveld = 0.0f;


	wallTravelAngle += rotationAngle;
	while (wallTravelAngle >= 90.0f) {
		if (!rotationChangedFlag) {
			if (rotationArg > 0)
				updateWallStructureClockwise();
			else
				updateWallStructureCounterClockwise();
		}
		else {
			if (wallTravelAngle >= 90.0f) 
				rotationChangedFlag = false;
			
		}
		wallTravelAngle -= 90.0f;

	}

	if (wallTravelAngle == 0) {
		restoreMiddleCubeTransofrmation();
		while (!middleCubeTemp.empty()) {
			middleCubeTemp.pop();
		}
	}

	InputManager::stopRotation();

	registerCallbackIfWallsNotAligned();

}

void InputManager::rotateWall(int index) {

	//Disable keyboard to avoid simultaneous wall rotations
	glfwSetKeyCallback(display.m_window, NULL);

	int* temp = nullptr;
	vec3 axis;

	switch (index)
	{
	case 0: {
		temp = wallsStructure.rightWall;
		axis = glm::cross(cubeUp, cubeForward);
	}break;
	case 1: {
		temp = wallsStructure.leftWall;
		axis = glm::cross(cubeUp, cubeForward);
	}break;
	case 2: {
		temp = wallsStructure.upperWall;
		axis = -cubeUp;
	}break;
	case 3: {
		temp = wallsStructure.lowerWall;
		axis = -cubeUp;
	}break;
	case 4: {
		temp = wallsStructure.backWall;
		axis = cubeForward;
	}break;
	case 5: {
		temp = wallsStructure.frontWall;
		axis = cubeForward;
	}break;
	}

	middleCubeTemp.push(transformations[temp[0]]);

	for (int i = 0; i < 9; i++) {
		cubeRotation[temp[i]] = glm::rotate(mat4(1), rotationArg, axis);
	}

	isWallRotationOn = true;
	wallIndex = index;
}

void InputManager::changeRotationAngle(bool indicator) {

	if (indicator) {
		if (rotationAngle > 25) {
			rotationAngle /= 2;
		}
	}
	else {
		if (rotationAngle < 180) {
			rotationAngle *= 2;
		}
	}



}

void InputManager::changeRotationDirection() {

	rotationArg = -rotationArg;

	if (wallTravelAngle != 0.0f) {
		wallTravelAngle -= 90.0f;
		wallTravelAngle = -wallTravelAngle;
		if (rotationChangedFlag == false) 
			rotationChangedFlag = true;
		
		else 
			rotationChangedFlag = false;
		

	}
}

int main(int argc, char** argv)
{

	Vertex vertices[] =
	{
		//	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal, const glm::vec3& color)
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(1, 0, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(0, 1, 1))
	};

	unsigned int indices[] = { 0, 1, 2,
							  0, 2, 3,

							  6, 5, 4,
							  7, 6, 4,

							  10, 9, 8,
							  11, 10, 8,

							  12, 13, 14,
							  12, 14, 15,

							  16, 17, 18,
							  16, 18, 19,

							  22, 21, 20,
							  23, 22, 20
	};

	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	Shader shader("./res/shaders/basicShader");



	//Perspective matrix
	mat4 P = glm::perspective(60.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f);
	P = P * glm::lookAt(pos, pos + forward, up);


	initializeMatrices();
	initializeWallStructure();
	glfwSetKeyCallback(display.m_window, InputManager::key_callback);

	while (!glfwWindowShouldClose(display.m_window))
	{
		Sleep(3);
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);

		for (int i = 0; i < 27; i++) {

			transformations[i] = cubeRotation[i] * transformations[i];
			mat4 MVP = P * transformations[i];
			shader.Bind();
			shader.Update(MVP, transformations[i]);
			mesh.Draw();

		}
		if (isWallRotationOn) {
			if (rotationAngleTraveld >= rotationAngle) {
				stopWallRotation();
			}
			rotationAngleTraveld += 0.5f;
		}

		display.SwapBuffers();

		glfwPollEvents();

	}

	return 0;
}



