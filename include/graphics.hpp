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
		void drawFrame();
		void waitIdle();
		//void setupResizeFunc();
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
		VkExtent2D swapChainExtent;
		VkPipelineLayout pipelineLayout;
		VkRenderPass renderPass;
		VkPipeline graphicsPipeline;
		VkCommandPool commandPool;
		bool frameBufferResized;
		size_t currentFrame;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		VkDebugUtilsMessengerEXT* debugger;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::vector<VkCommandBuffer> commandBuffers;

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
		static const size_t MAX_FRAMES_IN_FLIGHT;

		//functions
		Vulkan (const Vulkan&);
		Vulkan& operator=(const Vulkan&);
		void createDebugger();
		void createSwapChain();
		void createInstance();
		void createRenderPass();
		void selectSuitableDevice(const std::vector<VkPhysicalDevice>);
		void getPhysicalDevice();
		void createLogicalDeviceAndQueue();
		void createSurface();
		void createImageViews();
		void createGraphicsPipeline();
		void createFramebuffers();
		void createCommandPool();
		void createCommandBuffers();
		void createSyncObjects();
		void recreateSwapChain();
		void cleanupSwapChain();
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
