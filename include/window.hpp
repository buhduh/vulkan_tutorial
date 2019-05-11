#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "utils.hpp"

class Window : utils::Uncopyable {
public:
	static Window& getWindow();
	const GLFWwindow* getActualWindow() const;
	bool run();
	int height;
	int width;
private:
	GLFWwindow* window;
	Window();
	~Window();
	static const int DEF_WIDTH;
	static const int DEF_HEIGHT;
};

#endif
