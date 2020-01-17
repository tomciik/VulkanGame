#ifndef __Device_h
#define __Device_h

	#include <cstdint>
	#include <vulkan\vulkan_core.h>
	#include <stdexcept>
	#include <vector>
	#include <optional>
	#include <set>


struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class Device {
public:
	
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

	bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char*> deviceExtensions);

	bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions);

	void pickPhysicalDevice(VkInstance instance, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const std::vector<const char*> deviceExtensions);

	void createLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const std::vector<const char*> deviceExtensions,
		const bool enableValidationLayers, const std::vector<const char*> validationLayers, VkDevice device, VkQueue graphicsQueue, VkQueue presentQueue);
};
#endif

