#include "Window.h"
#include "InitVulkan.h"
#include "Device.h"
#include "MainLoop.h"
#include "Cleanup.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


int main() {

	InitVulkan initVulkan;
	Device devices;
	MainLoop mainLoop;
	CleanUp cleanUp;
	Window Window;

	//zmienne
	GLFWwindow* window = glfwCreateWindow(0, 0, "", nullptr, nullptr);

	VkInstance instance = 0;
	VkDebugUtilsMessengerEXT debugMessenger = 0;
	VkSurfaceKHR surface = 0;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = 0;

	VkQueue graphicsQueue = 0;
	VkQueue presentQueue = 0;

	VkSwapchainKHR swapChain = 0;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat = {};
	VkExtent2D swapChainExtent = {};
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass = 0;
	VkDescriptorSetLayout descriptorSetLayout = 0;
	VkPipelineLayout pipelineLayout = 0;
	VkPipeline graphicsPipeline = 0;

	VkCommandPool commandPool = 0;

	VkImage depthImage = 0;
	VkDeviceMemory depthImageMemory = 0;
	VkImageView depthImageView = 0;

	VkImage textureImage = 0;
	VkDeviceMemory textureImageMemory = 0;
	VkImageView textureImageView = 0;
	VkSampler textureSampler = 0;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	VkBuffer vertexBuffer = 0;
	VkDeviceMemory vertexBufferMemory = 0;
	VkBuffer indexBuffer = 0;
	VkDeviceMemory indexBufferMemory = 0;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkDescriptorPool descriptorPool = 0;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	bool framebufferResized = false;

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	const std::string MODEL_PATH = "models/chalet.obj";
	const std::string TEXTURE_PATH = "textures/chalet.jpg";

	Window.initWindow(window, 800, 600);


	initVulkan.createInstance(enableValidationLayers, validationLayers, instance);
	initVulkan.setupDebugMessenger(enableValidationLayers, instance, debugMessenger);
	
	initVulkan.createSurface(instance, window, surface);

	devices.pickPhysicalDevice(instance, physicalDevice, surface, deviceExtensions);
	devices.createLogicalDevice(physicalDevice, surface, deviceExtensions, enableValidationLayers, validationLayers, device, graphicsQueue, presentQueue);

	initVulkan.createSwapChain(physicalDevice, surface, window, device, swapChain, swapChainImages, swapChainExtent, swapChainImageFormat);
	initVulkan.createImageViews(swapChainImageViews, swapChainImages, device, swapChainImageFormat);
	initVulkan.createRenderPass(swapChainImageFormat, device, renderPass, physicalDevice);
	initVulkan.createDescriptorSetLayout(device, descriptorSetLayout);
	initVulkan.createGraphicsPipeline(device, swapChainExtent, descriptorSetLayout, pipelineLayout, renderPass, graphicsPipeline);
	initVulkan.createCommandPool(physicalDevice, device, surface, commandPool);
	initVulkan.createDepthResources(swapChainExtent, physicalDevice, depthImage, depthImageView, depthImageMemory, device);
	initVulkan.createFramebuffers(swapChainFramebuffers, swapChainImageViews, depthImageView, renderPass, swapChainExtent, device);
	initVulkan.createTextureImage(TEXTURE_PATH, device, commandPool, graphicsQueue, textureImage, textureImageMemory, physicalDevice);
	initVulkan.createTextureImageView(textureImageView, textureImage, device);
	initVulkan.createTextureSampler(device, textureSampler);
	initVulkan.loadModel(MODEL_PATH, vertices, indices);
	initVulkan.createVertexBuffer(vertices, device, commandPool, graphicsQueue, vertexBuffer, vertexBufferMemory, physicalDevice);
	initVulkan.createIndexBuffer(indices, device, commandPool, graphicsQueue, indexBuffer, indexBufferMemory, physicalDevice);
	initVulkan.createUniformBuffers(uniformBuffers, uniformBuffersMemory, swapChainImages, physicalDevice, device);
	initVulkan.createDescriptorPool(swapChainImages, device, descriptorPool);
	initVulkan.createDescriptorSets(swapChainImages, descriptorSetLayout, descriptorPool, descriptorSets, device, uniformBuffers, textureImageView, textureSampler);
	initVulkan.createCommandBuffers(commandBuffers, swapChainFramebuffers, commandPool, device, renderPass, swapChainExtent, graphicsPipeline, vertexBuffer, indexBuffer, pipelineLayout, descriptorSets, indices);
	initVulkan.createSyncObjects(imageAvailableSemaphores, renderFinishedSemaphores, inFlightFences, imagesInFlight, 2, swapChainImages, device);

	mainLoop.mainLoop(device, inFlightFences, imagesInFlight, currentFrame, swapChain, imageAvailableSemaphores, renderFinishedSemaphores, window,
		depthImageView, depthImage, depthImageMemory, swapChainFramebuffers, commandPool, commandBuffers, graphicsPipeline, pipelineLayout, renderPass,
		swapChainImageViews, swapChainImages, uniformBuffers, uniformBuffersMemory, framebufferResized, descriptorPool, physicalDevice, surface, graphicsQueue,
		presentQueue, swapChainExtent, swapChainImageFormat, descriptorSetLayout, descriptorSets, textureImageView, textureSampler, vertexBuffer,
		indexBuffer, indices, 2);

	cleanUp.cleanup(device, depthImageView, depthImage, depthImageMemory, swapChainFramebuffers, commandPool, commandBuffers, graphicsPipeline,
		pipelineLayout, renderPass, swapChainImageViews, swapChain, swapChainImages, uniformBuffers, uniformBuffersMemory, descriptorPool, textureSampler,
		textureImageView, textureImage, textureImageMemory, descriptorSetLayout, indexBuffer, vertexBufferMemory, vertexBuffer, indexBufferMemory, 2,
		renderFinishedSemaphores, imageAvailableSemaphores, inFlightFences, instance, surface, debugMessenger, enableValidationLayers, window);
}
