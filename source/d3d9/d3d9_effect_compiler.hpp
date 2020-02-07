/**
 * Copyright (C) 2014 Patrick Mours. All rights reserved.
 * License: https://github.com/crosire/reshade#license
 */

#pragma once

#include "effect_parser.hpp"
#include <sstream>
#include <unordered_set>
#include <spirv_hlsl.hpp>

namespace reshade::d3d9
{
	#pragma region Forward Declarations
	struct d3d9_sampler;
	struct d3d9_pass_data;
	class d3d9_runtime;
	#pragma endregion

	class d3d9_effect_compiler
	{
	public:
		d3d9_effect_compiler(d3d9_runtime *runtime, const reshadefx::module &module, std::string &errors, bool skipoptimization = false);

		bool run();

	private:
		void error(const std::string &message);
		void warning(const std::string &message);

		void visit_texture(const reshadefx::texture_info &texture_info);
		void visit_sampler(const reshadefx::sampler_info &sampler_info);
		void visit_uniform(const spirv_cross::CompilerHLSL &cross, const reshadefx::uniform_info &uniform_info);
		void visit_technique(const reshadefx::technique_info &technique_info);

		void compile_entry_point(spirv_cross::CompilerHLSL &cross, const spirv_cross::EntryPoint &entry);

		d3d9_runtime *_runtime;
		const reshadefx::module *_module;
		bool _success = true;
		std::string &_errors;
		size_t _uniform_storage_offset = 0, _constant_register_count = 0;
		std::stringstream _global_code, _global_uniforms;
		bool _skip_shader_optimization;
		HMODULE _d3dcompiler_module = nullptr;
		std::vector<d3d9_sampler> _sampler_bindings;
		std::unordered_map<std::string, com_ptr<IDirect3DVertexShader9>> vs_entry_points;
		std::unordered_map<std::string, com_ptr< IDirect3DPixelShader9>> ps_entry_points;
	};
}
