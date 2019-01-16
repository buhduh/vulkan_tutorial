#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "window.h"

namespace UX {
	//TODO this needs some work
	void initWindow(Window *window) {
		glfwInit();
		auto monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		window->window = glfwCreateWindow(640, 640, "Vulkan window", nullptr, nullptr);
    	//window->window = glfwCreateWindow(mode->width, mode->height, "Vulkan window", monitor, nullptr);
	}

	bool run(Window *window) {
		glfwPollEvents();
		return glfwWindowShouldClose(window->window);
	}

	//not a big fan of this...
	void cleanUp(Window *window) {
		glfwDestroyWindow(window->window);
		glfwTerminate();
	}
}
