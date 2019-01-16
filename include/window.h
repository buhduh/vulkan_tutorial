#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace UX {
	struct Window {
		GLFWwindow* window;
	};
	void initWindow(Window*);
	bool run(Window*);
	void cleanUp(Window*);
}

#endif
