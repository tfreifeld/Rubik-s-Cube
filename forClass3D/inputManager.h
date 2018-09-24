#pragma once   //maybe should be static class
#include "GLFW\glfw3.h"


class InputManager {

public:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void InputManager::key_callback_R(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void InputManager::key_callback_L(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void InputManager::key_callback_U(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void InputManager::key_callback_D(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void InputManager::key_callback_B(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void InputManager::key_callback_F(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void InputManager::key_callback_release(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void key_callback_RotationChange(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void InputManager::rotateAboutXAxis(bool direction);
	static void InputManager::rotateAboutYAxis(bool direction);
	static void InputManager::stopRotation();
	static void InputManager::rotateWall(int index);
	static void InputManager::changeRotationAngle(bool indicator);
	static void InputManager::changeRotationDirection();



};




