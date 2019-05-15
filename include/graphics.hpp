#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <string>
#include <array>
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan.h>

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
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		VkCommandPool commandPool;
		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
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
		static const std::vector<struct Vertex> vertices;
		static const std::vector<uint16_t> indices;

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
		void createUniformBuffers();
		void createGraphicsPipeline();
		void createFramebuffers();
		void createCommandPool();
		void createCommandBuffers();
		void createSyncObjects();
		void recreateSwapChain();
		void cleanupSwapChain();
		void createVertexBuffer();
		void createIndexBuffer();
		void createDescriptorSetLayout();
		void updateUniformBuffer(uint32_t);
		void createDescriptorPool();
		void createDescriptorSets();
		void createBuffer(
			VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags, 
			VkBuffer&, VkDeviceMemory&
		);
		void copyBuffer(VkBuffer, VkBuffer, VkDeviceSize);
		uint32_t findMemoryType(uint32_t, VkMemoryPropertyFlags);
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

struct Vertex {	
	glm::vec2 pos;
	glm::vec3 color;
	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);
		return attributeDescriptions;
	}

};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

#endif
