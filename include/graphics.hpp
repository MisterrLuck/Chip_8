#include <GLFW/glfw3.h>


class Graphics
{
public:
	bool init();

	bool updateFrame();
	void setOpen();

	~Graphics();

private:
	GLFWwindow* window;
	bool open = false;

	void onKey(int key, int scancode, int action, int mods);
	static void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
};
