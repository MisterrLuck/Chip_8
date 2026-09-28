#include <iostream>
// #include "../dependencies/GLFW/glfw3.h"
#include <GLFW/glfw3.h>
#include "SDL.h"
#include "chip8.hpp"
using std::cout;

bool window_open = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		window_open = false;
}

int main()
{
	Chip8 chip8;

	glfwInit();

	// Makes it floating in i3wm
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(640, 480, "Chip 8", NULL, NULL);

	glfwSetKeyCallback(window, key_callback);

	if (window)
	{
		window_open = true;
		while (!glfwWindowShouldClose(window) && window_open)
		{
			// Keep running
			glfwPollEvents();
		}
	} else
	{
		cout << "Error\n";
	}
	cout << "Destroyed\n";
	glfwDestroyWindow(window);

	return 0;
}
