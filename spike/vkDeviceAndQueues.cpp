#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

int main(void) {
	glfwInit();
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Device Spicke";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	VkInstance instance;
	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	VkPhysicalDevice physicalDevice;
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr); std::cout << "Device Count: " << deviceCount << std::endl; std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
	for(uint32_t i = 0; i < deviceCount; ++i) {
		auto curr = devices[i];
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(curr, &deviceProperties);
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(curr, &deviceFeatures);
		std::cout << "deviceProperties.apiVersion: " << deviceProperties.apiVersion << std::endl;
		std::cout << "deviceProperties.driverVersion: " << deviceProperties.driverVersion << std::endl;
		std::cout << "deviceProperties.vendorID: " << deviceProperties.vendorID << std::endl;
		std::cout << "deviceProperties.deviceID: " << deviceProperties.deviceID << std::endl;
		std::cout << "deviceProperties.deviceName: " << deviceProperties.deviceName << std::endl;
		std::cout << "	deviceFeatures.robustBufferAccess:                      " << deviceFeatures.robustBufferAccess << std::endl;
		std::cout << "	deviceFeatures.fullDrawIndexUint32:                     " << deviceFeatures.fullDrawIndexUint32 << std::endl;
		std::cout << "	deviceFeatures.imageCubeArray:                          " << deviceFeatures.imageCubeArray << std::endl;
		std::cout << "	deviceFeatures.independentBlend:                        " << deviceFeatures.independentBlend << std::endl;
		std::cout << "	deviceFeatures.geometryShader:                          " << deviceFeatures.geometryShader << std::endl;
		std::cout << "	deviceFeatures.tessellationShader:                      " << deviceFeatures.tessellationShader << std::endl;
		std::cout << "	deviceFeatures.sampleRateShading:                       " << deviceFeatures.sampleRateShading << std::endl;
		std::cout << "	deviceFeatures.dualSrcBlend:                            " << deviceFeatures.dualSrcBlend << std::endl;
		std::cout << "	deviceFeatures.logicOp:                                 " << deviceFeatures.logicOp << std::endl;
		std::cout << "	deviceFeatures.multiDrawIndirect:                       " << deviceFeatures.multiDrawIndirect << std::endl;
		std::cout << "	deviceFeatures.drawIndirectFirstInstance:               " << deviceFeatures.drawIndirectFirstInstance << std::endl;
		std::cout << "	deviceFeatures.depthClamp:                              " << deviceFeatures.depthClamp << std::endl;
		std::cout << "	deviceFeatures.depthBiasClamp:                          " << deviceFeatures.depthBiasClamp << std::endl;
		std::cout << "	deviceFeatures.fillModeNonSolid:                        " << deviceFeatures.fillModeNonSolid << std::endl;
		std::cout << "	deviceFeatures.depthBounds:                             " << deviceFeatures.depthBounds << std::endl;
		std::cout << "	deviceFeatures.wideLines:                               " << deviceFeatures.wideLines << std::endl;
		std::cout << "	deviceFeatures.largePoints:                             " << deviceFeatures.largePoints << std::endl;
		std::cout << "	deviceFeatures.alphaToOne:                              " << deviceFeatures.alphaToOne << std::endl;
		std::cout << "	deviceFeatures.multiViewport:                           " << deviceFeatures.multiViewport << std::endl;
		std::cout << "	deviceFeatures.samplerAnisotropy:                       " << deviceFeatures.samplerAnisotropy << std::endl;
		std::cout << "	deviceFeatures.textureCompressionETC2:                  " << deviceFeatures.textureCompressionETC2 << std::endl;
		std::cout << "	deviceFeatures.textureCompressionASTC_LDR:              " << deviceFeatures.textureCompressionASTC_LDR << std::endl;
		std::cout << "	deviceFeatures.textureCompressionBC:                    " << deviceFeatures.textureCompressionBC << std::endl;
		std::cout << "	deviceFeatures.occlusionQueryPrecise:                   " << deviceFeatures.occlusionQueryPrecise << std::endl;
		std::cout << "	deviceFeatures.pipelineStatisticsQuery:                 " << deviceFeatures.pipelineStatisticsQuery << std::endl;
		std::cout << "	deviceFeatures.vertexPipelineStoresAndAtomics:          " << deviceFeatures.vertexPipelineStoresAndAtomics << std::endl;
		std::cout << "	deviceFeatures.fragmentStoresAndAtomics:                " << deviceFeatures.fragmentStoresAndAtomics << std::endl;
		std::cout << "	deviceFeatures.shaderTessellationAndGeometryPointSize:  " << deviceFeatures.shaderTessellationAndGeometryPointSize << std::endl;
		std::cout << "	deviceFeatures.shaderImageGatherExtended:               " << deviceFeatures.shaderImageGatherExtended << std::endl;
		std::cout << "	deviceFeatures.shaderStorageImageExtendedFormats:       " << deviceFeatures.shaderStorageImageExtendedFormats << std::endl;
		std::cout << "	deviceFeatures.shaderStorageImageMultisample:           " << deviceFeatures.shaderStorageImageMultisample << std::endl;
		std::cout << "	deviceFeatures.shaderStorageImageReadWithoutFormat:     " << deviceFeatures.shaderStorageImageReadWithoutFormat << std::endl;
		std::cout << "	deviceFeatures.shaderStorageImageWriteWithoutFormat:    " << deviceFeatures.shaderStorageImageWriteWithoutFormat << std::endl;
		std::cout << "	deviceFeatures.shaderUniformBufferArrayDynamicIndexing: " << deviceFeatures.shaderUniformBufferArrayDynamicIndexing << std::endl;
		std::cout << "	deviceFeatures.shaderSampledImageArrayDynamicIndexing:  " << deviceFeatures.shaderSampledImageArrayDynamicIndexing << std::endl;
		std::cout << "	deviceFeatures.shaderStorageBufferArrayDynamicIndexing: " << deviceFeatures.shaderStorageBufferArrayDynamicIndexing << std::endl;
		std::cout << "	deviceFeatures.shaderStorageImageArrayDynamicIndexing:  " << deviceFeatures.shaderStorageImageArrayDynamicIndexing << std::endl;
		std::cout << "	deviceFeatures.shaderClipDistance:                      " << deviceFeatures.shaderClipDistance << std::endl;
		std::cout << "	deviceFeatures.shaderCullDistance:                      " << deviceFeatures.shaderCullDistance << std::endl;
		std::cout << "	deviceFeatures.shaderFloat64:                           " << deviceFeatures.shaderFloat64 << std::endl;
		std::cout << "	deviceFeatures.shaderInt64:                             " << deviceFeatures.shaderInt64 << std::endl;
		std::cout << "	deviceFeatures.shaderInt16:                             " << deviceFeatures.shaderInt16 << std::endl;
		std::cout << "	deviceFeatures.shaderResourceResidency:                 " << deviceFeatures.shaderResourceResidency << std::endl;
		std::cout << "	deviceFeatures.shaderResourceMinLod:                    " << deviceFeatures.shaderResourceMinLod << std::endl;
		std::cout << "	deviceFeatures.sparseBinding:                           " << deviceFeatures.sparseBinding << std::endl;
		std::cout << "	deviceFeatures.sparseResidencyBuffer:                   " << deviceFeatures.sparseResidencyBuffer << std::endl;
		std::cout << "	deviceFeatures.sparseResidencyImage2D:                  " << deviceFeatures.sparseResidencyImage2D << std::endl;
		std::cout << "	deviceFeatures.sparseResidencyImage3D:                  " << deviceFeatures.sparseResidencyImage3D << std::endl;
		std::cout << "	deviceFeatures.sparseResidency2Samples:                 " << deviceFeatures.sparseResidency2Samples << std::endl;
		std::cout << "	deviceFeatures.sparseResidency4Samples:                 " << deviceFeatures.sparseResidency4Samples << std::endl;
		std::cout << "	deviceFeatures.sparseResidency8Samples:                 " << deviceFeatures.sparseResidency8Samples << std::endl;
		std::cout << "	deviceFeatures.sparseResidency16Samples:                " << deviceFeatures.sparseResidency16Samples << std::endl;
		std::cout << "	deviceFeatures.sparseResidencyAliased:                  " << deviceFeatures.sparseResidencyAliased << std::endl;
		std::cout << "	deviceFeatures.variableMultisampleRate:                 " << deviceFeatures.variableMultisampleRate << std::endl;
		std::cout << "	deviceFeatures.inheritedQueries:                        " << deviceFeatures.inheritedQueries << std::endl;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(curr, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(curr, &queueFamilyCount, queueFamilies.data());
		std::cout << "\nqueue family count: " << queueFamilyCount << std::endl;
		for(uint32_t j = 0; j < queueFamilyCount; ++j) {
			std::cout << "******************" << std::endl;
			std::cout << "queue family iteration value: " << j << std::endl;
			auto q = queueFamilies[j];
			if(q.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				std::cout << "	VK_QUEUE_GRAPHICS_BIT" << std::endl;
			}
			if(q.queueFlags & VK_QUEUE_COMPUTE_BIT) {
				std::cout << "	VK_QUEUE_COMPUTE_BIT" << std::endl;
			}
			if(q.queueFlags & VK_QUEUE_TRANSFER_BIT) {
				std::cout << "	VK_QUEUE_TRANSFER_BIT" << std::endl;
			}
			if(q.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
				std::cout << "	VK_QUEUE_SPARSE_BINDING_BIT" << std::endl;
			}
			if(q.queueFlags & VK_QUEUE_PROTECTED_BIT) {
				std::cout << "	VK_QUEUE_PROTECTED_BIT" << std::endl;
			}
			std::cout << "\tq.queueCout: " << q.queueCount << std::endl;
			std::cout << "\tq.timestampValidBits: " << q.timestampValidBits << std::endl;
		}
	}
}
