#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

int main(void) {
	glfwInit();
	uint32_t lCount = 0;
	vkEnumerateInstanceLayerProperties(&lCount, nullptr);
	std::vector<VkLayerProperties> layers(lCount);	
	vkEnumerateInstanceLayerProperties(&lCount, layers.data());
	for (const auto& layer : layers) {
		std::cout << layer.layerName << std::endl;
		std::cout << "\t" << layer.description << std::endl;
		std::cout << "*****************" << std::endl;
	}
}
