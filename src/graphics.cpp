#include "graphics.hpp"

Graphics::~Graphics()
{
	glfwDestroyWindow(window);
}

bool Graphics::init()
{
	glfwInit();

	// Makes it floating in i3wm
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(640, 480, "Chip 8", NULL, NULL);

	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, onKey);
	
	glfwMakeContextCurrent(window);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	open = (bool) window;
	return open;
}

bool Graphics::updateFrame()
{
	int width, height;

	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(window);

	// Keep running
	glfwPollEvents();

	return open;
}

void Graphics::onKey(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		open = false;
}

// Static member function
// NOTE: This is a weird workaround to be able to modify the class members
void Graphics::onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Graphics* gfx =  reinterpret_cast<Graphics *>(glfwGetWindowUserPointer(window));
	gfx->onKey(key, scancode, action, mods);
}
