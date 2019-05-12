#ifndef WINDOW_H
#define WINDOW_H

#include <functional>

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
	void getFrameBufferSize(int*, int*);
	//this is fucked
	//void setupResizeFunc(std::function<void(GLFWwindow*, int, int)>);
private:
	GLFWwindow* window;
	Window();
	~Window();
	static const int DEF_WIDTH;
	static const int DEF_HEIGHT;
};

#endif
