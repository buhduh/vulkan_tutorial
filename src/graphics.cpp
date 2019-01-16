#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>

#include "applicationexception.h"
#include "graphics.h"

namespace Graphics {

	const std::string Vulkan::NO_VULKAN_INSTANCE("No Vulkan instance found.");
	const std::string Vulkan::NO_VULKAN_SURFACE("No Vulkan surface found.");
	const std::string Vulkan::NO_VULKAN_PHYSICAL_DEVICES("No Vulkan physical devices found.");
	const std::string Vulkan::NO_VULKAN_QUEUES("No queues found for device.");
	const std::string Vulkan::NO_VULKAN_DEVICE("No Vulkan logical device found.");
	const std::string Vulkan::NO_VULKAN_INITIALIZATION("Falied to initialize the Vulkan API.");
	const std::string Vulkan::NO_VULKAN_DEBUGGER("Failed to create Vulkan debug callback.");

	//TODO debug flags!!!!
	Vulkan::Vulkan(GLFWwindow* window)
			: instance(VK_NULL_HANDLE)
			, physicalDevice(VK_NULL_HANDLE)
			, device(VK_NULL_HANDLE)
			, graphicsQueue(VK_NULL_HANDLE)
			, presentQueue(VK_NULL_HANDLE)
			, surface(VK_NULL_HANDLE) {
		this->initVulkan();
		this->createSurface(window);
		this->createDebugger();
		this->getPhysicalDevice();
		this->createLogicalDeviceAndQueue();
	}

	void Vulkan::createSurface(GLFWwindow* window) {
		if(this->instance == VK_NULL_HANDLE) {
			throw ApplicationVulkanException(NO_VULKAN_INSTANCE);
		}
		if(glfwCreateWindowSurface(
			this->instance, window, nullptr, &this->surface) != VK_SUCCESS) {
			throw ApplicationVulkanException(NO_VULKAN_SURFACE);
		}
		if(this->surface == VK_NULL_HANDLE) {
			throw ApplicationVulkanException(NO_VULKAN_SURFACE);
		}
	}

	//TODO debug flag
	Vulkan::~Vulkan() {
		vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
		free(this->debugger);
		vkDestroyInstance(this->instance, nullptr);
	}

	void Vulkan::createLogicalDeviceAndQueue() {
		if(this->physicalDevice == VK_NULL_HANDLE) {
			throw ApplicationVulkanException(NO_VULKAN_PHYSICAL_DEVICES);
		}
		//Some stupid large number should be good
		uint32_t qFamilyIndex = pickQueueFamilyIndex();
		if(qFamilyIndex == UINT32_MAX) {
			throw ApplicationVulkanException(NO_VULKAN_QUEUES);
		}

		//I only need one, I think... My selection logic isn't as
		//sophisticated as the tutorials, and that mostly only uses one too
		//see https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Window_surface
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = qFamilyIndex;
		queueCreateInfo.queueCount = 1;
		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkDeviceCreateInfo dCreateInfo = {};
		dCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		dCreateInfo.pNext = nullptr;
		dCreateInfo.queueCreateInfoCount = 1;
		dCreateInfo.pQueueCreateInfos = &queueCreateInfo;
		VkPhysicalDeviceFeatures features = {};
		dCreateInfo.pEnabledFeatures = &features;
		auto dExtensions = getDeviceExtensions();
		dCreateInfo.enabledExtensionCount = dExtensions.size();
		dCreateInfo.ppEnabledExtensionNames = dExtensions.data();

		if(vkCreateDevice(
			this->physicalDevice, &dCreateInfo, 
			nullptr, &this->device) != VK_SUCCESS) {
			throw ApplicationVulkanException(NO_VULKAN_DEVICE);
		}
		if(this->device == VK_NULL_HANDLE) {
			throw ApplicationVulkanException(NO_VULKAN_DEVICE);
		}
		//TODO, these are the same, queue selection will need to be revisited for more
		//sophistication
		vkGetDeviceQueue(this->device, qFamilyIndex, 0, &this->graphicsQueue);
		vkGetDeviceQueue(this->device, qFamilyIndex, 0, &this->presentQueue);
		if(this->graphicsQueue == VK_NULL_HANDLE) {
			throw ApplicationVulkanException(NO_VULKAN_QUEUES);
		}
	}

	void Vulkan::getPhysicalDevice() {
		uint32_t dCount = 0;
		vkEnumeratePhysicalDevices(this->instance, &dCount, nullptr);
		if(dCount == 0) {
			throw ApplicationVulkanException(NO_VULKAN_PHYSICAL_DEVICES);
		}
		std::vector<VkPhysicalDevice> devices(dCount);
		vkEnumeratePhysicalDevices(this->instance, &dCount, devices.data());
		this->selectSuitableDevice(devices);
	}

	//TODO Ideally, this and selectSuitableDevice should leverage the same
	//queuefamily selection
	uint32_t Vulkan::pickQueueFamilyIndex() {
		if(this->physicalDevice == VK_NULL_HANDLE) {
			throw ApplicationVulkanException(NO_VULKAN_PHYSICAL_DEVICES);
		}
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(
			this->physicalDevice, &queueFamilyCount, nullptr);
		if(!queueFamilyCount) {
			throw ApplicationVulkanException(NO_VULKAN_QUEUES);
		}
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(
			this->physicalDevice, &queueFamilyCount, queueFamilies.data());
		for(uint32_t i = 0; i < queueFamilyCount; ++i) {
			auto q = queueFamilies[i];
			VkBool32 presentSupport = false;
			if(vkGetPhysicalDeviceSurfaceSupportKHR(
				this->physicalDevice, i, 
				this->surface, &presentSupport) != VK_SUCCESS) {
				continue;
			}
			if(q.queueCount && q.queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupport) {
				return i;
			}
		}
		return UINT32_MAX;
	}

	//TODO merge with pickQueueFamilyIndex for queue family index
	void Vulkan::selectSuitableDevice(const std::vector<VkPhysicalDevice> devices) {
		for(auto d : devices) {
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(d, &deviceProperties);
			//Selecting a descrete GPU is good enough for now
			if(deviceProperties.deviceType & VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				uint32_t queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(d, &queueFamilyCount, nullptr);
				if(!queueFamilyCount) {
					continue;
				}
				std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(
					d, &queueFamilyCount, queueFamilies.data());
				vkGetPhysicalDeviceQueueFamilyProperties(
					d, &queueFamilyCount, nullptr);
				for(auto q : queueFamilies) {
					if(q.queueCount && q.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						this->physicalDevice = d;
						return;
					}
				}
			}
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL Vulkan::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		void* pUserData
	) {
        std::cout << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

	void Vulkan::createInstance() {
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Drone";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		/*
		Extensions
		*/
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		//required for layer callbacks
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		createInfo.enabledExtensionCount = extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();
		/*
		Extensions
		*/

		/*
		Layers
		*/
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_LUNARG_standard_validation"
		};	
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		/*
		Layers
		*/

		VkResult result = vkCreateInstance(&createInfo, nullptr, &this->instance);
		if(result != VK_SUCCESS) {
			throw ApplicationVulkanException(NO_VULKAN_INITIALIZATION);
		}
	}

	//TODO debugging switches
	void Vulkan::initVulkan() {
		this->createInstance();
	}

	void Vulkan::createDebugger() {
		this->debugger = (VkDebugUtilsMessengerEXT*) 
			malloc(sizeof(VkDebugUtilsMessengerEXT));
		VkDebugUtilsMessengerCreateInfoEXT cInfo = {};
		cInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		cInfo.pNext = nullptr;
		cInfo.flags = 0;
		cInfo.messageSeverity = 
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		cInfo.messageType = 
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT; 
		cInfo.pfnUserCallback = debugCallback;
		auto vkCreateDebugUtilsMessengerEXT = 
			(PFN_vkCreateDebugUtilsMessengerEXT) 
				vkGetInstanceProcAddr(this->instance, "vkCreateDebugUtilsMessengerEXT");
		auto res = vkCreateDebugUtilsMessengerEXT(
			this->instance, &cInfo, nullptr, this->debugger);
		if(res != VK_SUCCESS) {
			throw ApplicationVulkanException(NO_VULKAN_DEBUGGER);
		}
	}
	
	std::vector<const char *> Vulkan::getDeviceExtensions() const {
		std::vector<const char *> toRet(1);
		toRet[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
		return toRet;
	}
}
