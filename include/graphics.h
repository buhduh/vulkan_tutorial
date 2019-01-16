#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

class Vulkan {
	public:
		Vulkan(GLFWwindow*);
		~Vulkan();
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT,
			VkDebugUtilsMessageTypeFlagsEXT,
			const VkDebugUtilsMessengerCallbackDataEXT*,
			void*
		);
	private:
		//members
		VkInstance instance;
		VkPhysicalDevice physicalDevice;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkSurfaceKHR surface;
		VkDebugUtilsMessengerEXT* debugger;

		//static members
		static const std::string NO_VULKAN_INSTANCE;
		static const std::string NO_VULKAN_SURFACE;
		static const std::string NO_VULKAN_PHYSICAL_DEVICES;
		static const std::string NO_VULKAN_QUEUES;
		static const std::string NO_VULKAN_DEVICE;
		static const std::string NO_VULKAN_INITIALIZATION;
		static const std::string NO_VULKAN_DEBUGGER;

		//functions
		void initVulkan();
		void createDebugger();
		void createInstance();
		void selectSuitableDevice(const std::vector<VkPhysicalDevice>);
		void getPhysicalDevice();
		void createLogicalDeviceAndQueue();
		uint32_t pickQueueFamilyIndex();
		void createSurface(GLFWwindow*);
		std::vector<const char *> getDeviceExtensions() const;
};
#endif
