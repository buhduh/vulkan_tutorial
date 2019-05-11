#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#include <set>

#include "applicationexception.hpp"
#include "graphics.hpp"

const std::string Vulkan::NO_VULKAN_INSTANCE("No Vulkan instance found.");
const std::string Vulkan::NO_VULKAN_SURFACE("No Vulkan surface found.");
const std::string Vulkan::NO_VULKAN_PHYSICAL_DEVICES("No Vulkan physical devices found.");
const std::string Vulkan::NO_VULKAN_QUEUES("No queues found for device.");
const std::string Vulkan::NO_VULKAN_DEVICE("No Vulkan logical device found.");
const std::string Vulkan::NO_VULKAN_INITIALIZATION("Falied to initialize the Vulkan API.");
const std::string Vulkan::NO_VULKAN_DEBUGGER("Failed to create Vulkan debug callback.");
const std::string Vulkan::NO_VULKAN_SURFACE_FORMATS("No physical surface formats found.");
const std::string Vulkan::NO_VULKAN_SURFACE_PRESENT_MODES("No physical device surface present modes.");
const std::string Vulkan::NO_VULKAN_SWAP_CHAIN("Failed to create swap chain.");
const std::string Vulkan::NO_VULKAN_SWAP_CHAIN_FORMAT("No vulkan swap chain formats found.");
const std::string Vulkan::NO_VULKAN_IMAGE_VIEW("Failed creating a vulkan image view.");
const std::string Vulkan::BAD_SHADER_MODULE("Failed creating creating shader module.");

const std::vector<const char*> Vulkan::deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

//TODO debug flags!!!!
Vulkan::Vulkan(Window& window)
		: instance(VK_NULL_HANDLE)
		, physicalDevice(VK_NULL_HANDLE)
		, device(VK_NULL_HANDLE)
		, graphicsQueue(VK_NULL_HANDLE)
		, presentQueue(VK_NULL_HANDLE)
		, surface(VK_NULL_HANDLE)
		, swapChain(VK_NULL_HANDLE)
		, swapChainImageFormat(VK_FORMAT_UNDEFINED)
		, swapChainImages(0)
		, swapChainImageViews(0)
		, window(window) {
	this->debugger = (VkDebugUtilsMessengerEXT*) 
		malloc(sizeof(VkDebugUtilsMessengerEXT));
	createInstance();
	createSurface();
	createDebugger();
	getPhysicalDevice();
	createLogicalDeviceAndQueue();
	createSwapChain();
	createImageViews();
	createGraphicsPipeline();
}

//TODO debug flag
Vulkan::~Vulkan() {
	for(auto i : swapChainImageViews) {
		vkDestroyImageView(device, i, nullptr);
	}
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
	free(this->debugger);
	vkDestroyInstance(this->instance, nullptr);
}

VkShaderModule Vulkan::createShaderModule(const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
	VkShaderModule shaderModule;
	if(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw ApplicationVulkanException(BAD_SHADER_MODULE);		
	}
	return shaderModule;
}

void Vulkan::createGraphicsPipeline() {
    auto vertShaderCode = utils::readFile("shaders/vert.spv");
    auto fragShaderCode = utils::readFile("shaders/frag.spv");
	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {
		vertShaderStageInfo, fragShaderStageInfo
	};

	VkPipelineVertexInputStateCreateInfo vInputInfo = {};
	vInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vInputInfo.vertexBindingDescriptionCount = 0;
	vInputInfo.pVertexBindingDescriptions = nullptr;
	vInputInfo.vertexAttributeDescriptionCount = 0;
	vInputInfo.pVertexAttributeDescriptions = nullptr;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) swapChainExtent.width;
	viewport.height = (float) swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional


	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

void Vulkan::createImageViews() {
	if(swapChain == VK_NULL_HANDLE) {
		throw ApplicationVulkanException(NO_VULKAN_SWAP_CHAIN);
	}
	if(swapChainImages.size() == 0) {
		throw ApplicationVulkanException(NO_VULKAN_SWAP_CHAIN);
	}
	if(swapChainImageFormat == VK_FORMAT_UNDEFINED) {
		throw ApplicationVulkanException(NO_VULKAN_SWAP_CHAIN);
	}
	swapChainImageViews.resize(swapChainImages.size());
	//TODO i have no idea if setting 
	for(uint32_t i = 0; i < swapChainImages.size(); ++i) {
		VkImageViewCreateInfo cInfo = {};
		cInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		cInfo.image = swapChainImages[i];
		cInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		cInfo.format = swapChainImageFormat;
		cInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		cInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		cInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		cInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		cInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		cInfo.subresourceRange.baseMipLevel = 0;
		cInfo.subresourceRange.levelCount = 1;
		cInfo.subresourceRange.baseArrayLayer = 0;
		cInfo.subresourceRange.layerCount = 1;
		if(vkCreateImageView(
			device, &cInfo, nullptr, 
			&swapChainImageViews[i]) != VK_SUCCESS) {
			throw ApplicationVulkanException(NO_VULKAN_IMAGE_VIEW);
		}
	}
}

void Vulkan::createSurface() {
	if(this->instance == VK_NULL_HANDLE) {
		throw ApplicationVulkanException(NO_VULKAN_INSTANCE);
	}
	const GLFWwindow* gWindow = (this->window).getActualWindow();
	//don't know if this modifies window or what....
	if(glfwCreateWindowSurface(
		this->instance, const_cast<GLFWwindow*>(gWindow), 
		nullptr, &this->surface) != VK_SUCCESS) {
		throw ApplicationVulkanException(NO_VULKAN_SURFACE);
	}
	if(this->surface == VK_NULL_HANDLE) {
		throw ApplicationVulkanException(NO_VULKAN_SURFACE);
	}
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
	dCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	dCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

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
	if(this->graphicsQueue == VK_NULL_HANDLE || 
		this->presentQueue == VK_NULL_HANDLE) {
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
	if(this->physicalDevice == VK_NULL_HANDLE)
		throw ApplicationVulkanException(NO_VULKAN_PHYSICAL_DEVICES);
}

uint32_t Vulkan::pickQueueFamilyIndex(VkPhysicalDevice pDevice) {
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(
		pDevice, &queueFamilyCount, nullptr);
	if(!queueFamilyCount) {
		throw ApplicationVulkanException(NO_VULKAN_QUEUES);
	}
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(
		pDevice, &queueFamilyCount, queueFamilies.data());
	for(uint32_t i = 0; i < queueFamilyCount; ++i) {
		auto q = queueFamilies[i];
		VkBool32 presentSupport = false;
		if(vkGetPhysicalDeviceSurfaceSupportKHR(
			pDevice, i, 
			this->surface, &presentSupport) != VK_SUCCESS) {
			continue;
		}
		if(q.queueCount && q.queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupport) {
			return i;
		}
	}
	return UINT32_MAX;
}

uint32_t Vulkan::pickQueueFamilyIndex() {
	if(this->physicalDevice == VK_NULL_HANDLE) {
		throw ApplicationVulkanException(NO_VULKAN_PHYSICAL_DEVICES);
	}
	return pickQueueFamilyIndex(this->physicalDevice);
}

SwapChainSupportDetails Vulkan::querySwapChainSupport(VkPhysicalDevice pDevice) {
	SwapChainSupportDetails details;
	if(this->surface == VK_NULL_HANDLE) {
		throw ApplicationVulkanException(NO_VULKAN_SURFACE);
	}
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		pDevice, this->surface, &details.capabilities);
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(
		pDevice, this->surface, &formatCount, nullptr);
	if(formatCount == 0) {
		throw ApplicationVulkanException(NO_VULKAN_SURFACE_FORMATS);
	}
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(
		pDevice, this->surface, 
		&formatCount, details.formats.data()
	);
	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(
		pDevice, this->surface, &presentModeCount, nullptr
	);
	if(presentModeCount == 0) {
		throw ApplicationVulkanException(NO_VULKAN_SURFACE_PRESENT_MODES);
	}
	details.presentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(
		pDevice, this->surface, 
		&presentModeCount, details.presentModes.data()
	);
    return details;
}

void Vulkan::selectSuitableDevice(const std::vector<VkPhysicalDevice> devices) {
	for(auto d : devices) {
		if(pickQueueFamilyIndex(d) == UINT32_MAX) continue;
		if(!checkDeviceExtensionSupport(d)) continue;
		SwapChainSupportDetails swapChainSupport = 
			querySwapChainSupport(d);
		if(
			swapChainSupport.formats.empty() ||
			swapChainSupport.presentModes.empty()) {
			continue;
		}
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(d, &deviceProperties);
		//Selecting a descrete GPU is good enough for now
		if(deviceProperties.deviceType & VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			this->physicalDevice = d;
			break;
		}
	}
}

VkSurfaceFormatKHR Vulkan::chooseSwapSurfaceFormat(
	const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	for(const auto& a : availableFormats) {
		if(a.format == VK_FORMAT_B8G8R8A8_UNORM && 
			a.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return a;
		}
	}
	return availableFormats[0];
}

VkPresentModeKHR Vulkan::chooseSwapPresentMode(
	const std::vector<VkPresentModeKHR> availablePresentModes) {
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
	for(const auto& a : availablePresentModes) {
        if(a == VK_PRESENT_MODE_MAILBOX_KHR) {
            return a;
        } else if(a == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            bestMode = a;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Vulkan::chooseSwapExtent(
	const VkSurfaceCapabilitiesKHR& capabilities) {
	    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
		uint32_t width = static_cast<uint32_t>(window.width);
		uint32_t height = static_cast<uint32_t>(window.height);
        VkExtent2D actualExtent = {width, height};
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}

void Vulkan::createSwapChain() {
	if(this->physicalDevice	== VK_NULL_HANDLE) {
		throw ApplicationVulkanException(NO_VULKAN_PHYSICAL_DEVICES);
	}
	if(this->surface == VK_NULL_HANDLE) {
		throw ApplicationVulkanException(NO_VULKAN_SURFACE);
	}
	SwapChainSupportDetails details = 
		querySwapChainSupport(this->physicalDevice);
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(details.formats);		
	VkPresentModeKHR presentMode = chooseSwapPresentMode(details.presentModes);
	VkExtent2D extent = chooseSwapExtent(details.capabilities);
	uint32_t imageCount = details.capabilities.minImageCount + 1;
	if(
		details.capabilities.maxImageCount && 
		imageCount > details.capabilities.maxImageCount
	) {
		imageCount = details.capabilities.maxImageCount;
	}
	VkSwapchainCreateInfoKHR cInfo = {};
	cInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	cInfo.surface = surface;
	cInfo.minImageCount = imageCount;
	cInfo.imageFormat = surfaceFormat.format;
	cInfo.imageColorSpace = surfaceFormat.colorSpace;
	cInfo.imageExtent = extent;
	cInfo.imageArrayLayers = 1;
	cInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
//NOTE this could change if graphicsFamily != presentFamily
	cInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	cInfo.queueFamilyIndexCount = 0;
	cInfo.pQueueFamilyIndices = nullptr;
	cInfo.preTransform = details.capabilities.currentTransform;
	cInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	cInfo.presentMode = presentMode;
	cInfo.clipped = VK_TRUE;
	cInfo.oldSwapchain = VK_NULL_HANDLE;
	if(vkCreateSwapchainKHR(device, &cInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw ApplicationVulkanException(NO_VULKAN_SWAP_CHAIN);
	}
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
	swapChainImageFormat = surfaceFormat.format;
}


bool Vulkan::checkDeviceExtensionSupport(VkPhysicalDevice pDevice) {
   uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(
		pDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, availableExtensions.data());
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for (const auto& extension : availableExtensions) {
		if(requiredExtensions.empty()) return true;
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
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
		"VK_LAYER_LUNARG_standard_validation",
		//TODO why don't I have VK_LAYER_KHRONOS_validation?
		//"VK_LAYER_KHRONOS_validation"
		"VK_LAYER_VALVE_steam_overlay_64"
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

void Vulkan::createDebugger() {
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
