/**
 * Copyright (C) 2014 Patrick Mours. All rights reserved.
 * License: https://github.com/crosire/reshade#license
 */

#pragma once

#include <vulkan/vulkan.h>
#include "runtime.hpp"

namespace reshade::vulkan
{
	class runtime_vulkan : public runtime
	{
	public:
		runtime_vulkan(VkDevice device, VkPhysicalDevice physical_device);

		bool on_init(VkSwapchainKHR swapchain, const VkSwapchainCreateInfoKHR &desc, HWND hwnd);
		void on_reset();
		void on_present(uint32_t swapchain_image_index);

		void capture_screenshot(uint8_t *buffer) const override;

	private:
		bool init_backbuffer_textures(const VkSwapchainCreateInfoKHR &desc);
		bool init_command_pools();

		bool init_texture(texture &texture);
		void upload_texture(texture &texture, const uint8_t *pixels);

		bool compile_effect(effect_data &effect);
		void unload_effects();

		void render_technique(technique &technique) override;

#if RESHADE_GUI
		bool init_imgui_resources();
		void render_imgui_draw_data(ImDrawData *draw_data) override;
#endif

		void generate_mipmaps(const VkCommandBuffer cmd_list, texture &texture);

		VkCommandBuffer create_command_list(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY) const;
		void execute_command_list(VkCommandBuffer cmd_list) const;
		void execute_command_list_async(VkCommandBuffer cmd_list) const;

		VkDevice _device;
		VkPhysicalDevice _physical_device;
		VkSwapchainKHR _swapchain;

		VkFence _wait_fence = VK_NULL_HANDLE;
		VkQueue _current_queue = VK_NULL_HANDLE;
		uint32_t _current_index = 0;

		std::vector<VkCommandPool> _cmd_pool;

		std::vector<VkImageView> _swapchain_views;
		std::vector<VkFramebuffer> _swapchain_frames;
		VkRenderPass _default_render_pass = VK_NULL_HANDLE;
		VkExtent2D _render_area = {};
		VkFormat _backbuffer_format = VK_FORMAT_UNDEFINED;

		PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR = nullptr;
		PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT = nullptr;

#if RESHADE_GUI
		unsigned int _imgui_index_buffer_size = 0;
		VkBuffer _imgui_index_buffer = VK_NULL_HANDLE;
		unsigned int _imgui_vertex_buffer_size = 0;
		VkBuffer _imgui_vertex_buffer = VK_NULL_HANDLE;
		VkSampler _imgui_font_sampler = VK_NULL_HANDLE;
		VkPipeline _imgui_pipeline = VK_NULL_HANDLE;
		VkPipelineLayout _imgui_pipeline_layout = VK_NULL_HANDLE;
		VkDescriptorSetLayout _imgui_descriptor_set_layout = VK_NULL_HANDLE;
		VkDeviceSize _imgui_vertex_mem_offset = 0;
		VkDeviceMemory _imgui_vertex_mem = VK_NULL_HANDLE;
#endif
	};
}