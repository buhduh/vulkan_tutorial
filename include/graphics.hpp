#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

#include "utils.hpp"
#include "window.hpp"

class Vulkan : private utils::Uncopyable {
	public:
		Vulkan(Window& window);
		~Vulkan();
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT,
			VkDebugUtilsMessageTypeFlagsEXT,
			const VkDebugUtilsMessengerCallbackDataEXT*,
			void*
		);
	private:
		//members
		Window& window;
		VkInstance instance;
		VkPhysicalDevice physicalDevice;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkSurfaceKHR surface;
		VkSwapchainKHR swapChain;
		VkFormat swapChainImageFormat;
		VkDebugUtilsMessengerEXT* debugger;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;

		//static members
		static const std::string NO_VULKAN_INSTANCE;
		static const std::string NO_VULKAN_SURFACE;
		static const std::string NO_VULKAN_PHYSICAL_DEVICES;
		static const std::string NO_VULKAN_QUEUES;
		static const std::string NO_VULKAN_DEVICE;
		static const std::string NO_VULKAN_INITIALIZATION;
		static const std::string NO_VULKAN_DEBUGGER;
		static const std::string NO_VULKAN_SURFACE_FORMATS;
		static const std::string NO_VULKAN_SURFACE_PRESENT_MODES;
		static const std::string NO_VULKAN_SWAP_CHAIN;
		static const std::string NO_VULKAN_SWAP_CHAIN_FORMAT;
		static const std::string NO_VULKAN_IMAGE_VIEW;
		static const std::string BAD_SHADER_MODULE;
		static const std::vector<const char*> deviceExtensions;

		//functions
		Vulkan (const Vulkan&);
		Vulkan& operator=(const Vulkan&);
		void createDebugger();
		void createSwapChain();
		void createInstance();
		void selectSuitableDevice(const std::vector<VkPhysicalDevice>);
		void getPhysicalDevice();
		void createLogicalDeviceAndQueue();
		void createSurface();
		void createImageViews();
		void createGraphicsPipeline();
		bool checkDeviceExtensionSupport(VkPhysicalDevice);
		uint32_t pickQueueFamilyIndex();
		uint32_t pickQueueFamilyIndex(VkPhysicalDevice);
		struct SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(
			const std::vector<VkSurfaceFormatKHR>&
		);
		VkPresentModeKHR chooseSwapPresentMode(
			const std::vector<VkPresentModeKHR>
		);
		VkExtent2D chooseSwapExtent(
			const VkSurfaceCapabilitiesKHR&
		);
		VkShaderModule createShaderModule(const std::vector<char>& code);
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

#endif
