#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "window.hpp"

#include <iostream>

const int Window::DEF_WIDTH(960);
const int Window::DEF_HEIGHT(540);

Window::Window() {
	glfwInit();
	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	this->window = glfwCreateWindow(DEF_WIDTH, DEF_HEIGHT, "Vulkan window", nullptr, nullptr);
	glfwGetFramebufferSize(this->window, &this->width, &this->height);
	//window->window = glfwCreateWindow(mode->width, mode->height, "Vulkan window", monitor, nullptr);
}

Window& Window::getWindow() {
	static Window instance;
	return instance;
}

const GLFWwindow* Window::getActualWindow() const {
	return this->window;
}

bool Window::run() {
	glfwPollEvents();
	return !glfwWindowShouldClose(this->window);
}

//TODO is this even working?
//this is fucked
/*
void Window::setupResizeFunc(std::function<void(GLFWwindow*, int, int)> cb) {
	don't think i need a lambda
	glfwSetFramebufferSizeCallback(window, [&, this](){
		cb();	
	});
	glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)cb;
}
*/

void Window::getFrameBufferSize(int* t_width, int* t_height) {
	glfwGetFramebufferSize(window, t_width, t_height);
	width = *t_width;
	height = *t_height;
}

Window::~Window() {
	glfwDestroyWindow(this->window);
	glfwTerminate();
}
