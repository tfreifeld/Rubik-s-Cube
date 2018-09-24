#include "inputManager.h"

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;	
	case GLFW_KEY_UP:
		if (action == GLFW_PRESS) {
			rotateAboutXAxis(true);
		}
		else if (action == GLFW_RELEASE)
		{
			stopRotation();
		}
		break;
	case GLFW_KEY_DOWN:
		if (action == GLFW_PRESS) {
			rotateAboutXAxis(false);
		}
		else if (action == GLFW_RELEASE)
		{
			stopRotation();
		}
		break;
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS) {
			rotateAboutYAxis(true);
		}
		else if (action == GLFW_RELEASE)
		{
			stopRotation();
		}
		break;
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS) {
			rotateAboutYAxis(false);
		}
		else if (action == GLFW_RELEASE)
		{
			stopRotation();
		}
		break;
	case GLFW_KEY_R:
		key_callback_R(window, key, scancode, action, mods);

		break;
	case GLFW_KEY_L:
		key_callback_L(window, key, scancode, action, mods);
		break;
	case GLFW_KEY_U:
		key_callback_U(window, key, scancode, action, mods);
		break;
	case GLFW_KEY_D:
		key_callback_D(window, key, scancode, action, mods);

		break;
	case GLFW_KEY_B:
		key_callback_B(window, key, scancode, action, mods);
		break;
	case GLFW_KEY_F:
		key_callback_F(window, key, scancode, action, mods);

		break;
	default:
		key_callback_RotationChange(window, key, scancode, action, mods);
		break;
	}
}

void InputManager::key_callback_R(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		rotateWall(0);
	}
	else
		key_callback_RotationChange(window, key, scancode, action, mods);

}

void InputManager::key_callback_L(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		rotateWall(1);
	}
	else
		key_callback_RotationChange(window, key, scancode, action, mods);

}

void InputManager::key_callback_U(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_U && action == GLFW_PRESS) {
		rotateWall(2);
	}
	else
		key_callback_RotationChange(window, key, scancode, action, mods);

}

void InputManager::key_callback_D(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		rotateWall(3);
	}
	else
		key_callback_RotationChange(window, key, scancode, action, mods);

}

void InputManager::key_callback_B(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		rotateWall(4);
	}
	else
		key_callback_RotationChange(window, key, scancode, action, mods);

}

void InputManager::key_callback_F(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		rotateWall(5);
	}
	else
		key_callback_RotationChange(window, key, scancode, action, mods);

}

void InputManager::key_callback_release(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action == GLFW_RELEASE) {
		stopRotation();
	}
}

void InputManager::key_callback_RotationChange(GLFWwindow* window, int key, int scancode, int action, int mods) {

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	
	case GLFW_KEY_Z:
		if (action == GLFW_PRESS) {
			changeRotationAngle(true);
		}
		break;
	case GLFW_KEY_A:
		if (action == GLFW_PRESS) {
			changeRotationAngle(false);
		}
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS) {
			changeRotationDirection();
		}
		break;
	default:
		break;
	}

}





