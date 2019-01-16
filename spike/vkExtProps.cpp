#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

int main(void) {
	glfwInit();
	uint32_t extCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extCount);	
	vkEnumerateInstanceExtensionProperties(nullptr, &extCount, extensions.data());
	std::cout << "all extensions" << std::endl;
	for (const auto& extension : extensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
	}
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> gEXT(glfwExtensions, glfwExtensions + glfwExtensionCount);
	std::cout << "glfw extensions" << std::endl;
	for (const auto& extension : gEXT) {
		std::cout << "\t" << extension << std::endl;
	}
}
