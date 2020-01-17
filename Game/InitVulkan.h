#ifndef __InitVulkan_h
#define __InitVulkan_h
	
	#define GLFW_INCLUDE_VULKAN
	#include <GLFW/glfw3.h>

	#define GLM_FORCE_RADIANS
	#define GLM_FORCE_DEPTH_ZERO_TO_ONE
	#define GLM_ENABLE_EXPERIMENTAL
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtx/hash.hpp>

	#include <iostream>
	#include <fstream>
	#include <stdexcept>
	#include <algorithm>
	#include <chrono>
	#include <vector>
	#include <cstring>
	#include <cstdlib>
	#include <cstdint>
	#include <array>
	#include <optional>
	#include <set>
	#include <unordered_map>


		struct Vertex {
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			return attributeDescriptions;
		}

		bool operator==(const Vertex& other) const {
			return pos == other.pos && color == other.color && texCoord == other.texCoord;
		}
	};

		struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	class InitVulkan {
		
		public:
			//TWORZENIE INSTANCJI
			void createInstance(const bool enableValidationLayers, const std::vector<const char*> validationLayers, VkInstance instance);

			//POWIADAMIANIE O PROBLEMACH
			void setupDebugMessenger(const bool enableValidationLayers, VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger);

			//
			void createSurface(VkInstance instance, GLFWwindow* window, VkSurfaceKHR surface);

			//
			void createSwapChain(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GLFWwindow* window, VkDevice device, VkSwapchainKHR swapChain,
				std::vector<VkImage> swapChainImages, VkExtent2D swapChainExtent, VkFormat swapChainImageFormat);

			//
			void createImageViews(std::vector<VkImageView> swapChainImageViews, std::vector<VkImage> swapChainImages, VkDevice device, VkFormat swapChainImageFormat);

			//
			void createRenderPass(VkFormat swapChainImageFormat, VkDevice device, VkRenderPass renderPass, VkPhysicalDevice physicalDevice);

			//
			void createDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout);

			//
			void createGraphicsPipeline(VkDevice device, VkExtent2D swapChainExtent, VkDescriptorSetLayout descriptorSetLayout, VkPipelineLayout pipelineLayout,
				VkRenderPass renderPass, VkPipeline graphicsPipeline);

			//
			void createCommandPool(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, VkCommandPool commandPool);

			//
			void createDepthResources(VkExtent2D swapChainExtent, VkPhysicalDevice physicalDevice, VkImage depthImage, VkImageView depthImageView, VkDeviceMemory depthImageMemory, VkDevice device);

			//
			void createFramebuffers(std::vector<VkFramebuffer> swapChainFramebuffers, std::vector<VkImageView> swapChainImageViews, VkImageView depthImageView,
				VkRenderPass renderPass, VkExtent2D swapChainExtent, VkDevice device);

			//
			void createTextureImage(const std::string TEXTURE_PATH, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue,
				VkImage textureImage, VkDeviceMemory textureImageMemory, VkPhysicalDevice physicalDevice);

			//
			void createTextureImageView(VkImageView textureImageView, VkImage textureImage, VkDevice device);

			//
			void createTextureSampler(VkDevice device, VkSampler textureSampler);

			//
			void loadModel(const std::string MODEL_PATH, std::vector<Vertex> vertices, std::vector<uint32_t> indices);

			//
			void createVertexBuffer(std::vector<Vertex> vertices, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue,
				VkBuffer vertexBuffer, VkDeviceMemory vertexBufferMemory, VkPhysicalDevice physicalDevice);

			//
			void createIndexBuffer(std::vector<uint32_t> indices, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue,
				VkBuffer indexBuffer, VkDeviceMemory indexBufferMemory, VkPhysicalDevice physicalDevice);

			//
			void createUniformBuffers(std::vector<VkBuffer> uniformBuffers, std::vector<VkDeviceMemory> uniformBuffersMemory, std::vector<VkImage> swapChainImages,
				VkPhysicalDevice physicalDevice, VkDevice device);

			//
			void createDescriptorPool(std::vector<VkImage> swapChainImages, VkDevice device, VkDescriptorPool descriptorPool);

			void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool,
				std::vector<VkDescriptorSet> descriptorSets, VkDevice device, std::vector<VkBuffer> uniformBuffers, VkImageView textureImageView, VkSampler textureSampler);

			//
			void createCommandBuffers(std::vector<VkCommandBuffer> commandBuffers, std::vector<VkFramebuffer> swapChainFramebuffers, VkCommandPool commandPool, VkDevice device,
				VkRenderPass renderPass, VkExtent2D swapChainExtent, VkPipeline graphicsPipeline, VkBuffer vertexBuffer, VkBuffer indexBuffer,
				VkPipelineLayout pipelineLayout, std::vector<VkDescriptorSet> descriptorSets, std::vector<uint32_t> indices);

			//
			void createSyncObjects(std::vector<VkSemaphore> imageAvailableSemaphores, std::vector<VkSemaphore> renderFinishedSemaphores, std::vector<VkFence> inFlightFences,
				std::vector<VkFence> imagesInFlight, const int MAX_FRAMES_IN_FLIGHT, std::vector<VkImage> swapChainImages, VkDevice device);
	};

#endif