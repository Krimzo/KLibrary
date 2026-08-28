#pragma once

#include "apis/apis.h"

namespace kl
{
struct CompiledShader
{
    dx::DataBlob data;
    dx::DataBlob error;

    void const* data_ptr() const;
    SIZE_T data_size() const;

    char const* error_ptr() const;
    SIZE_T error_size() const;
};
} // namespace kl

namespace kl
{
struct ShaderCompiler
{
    CompiledShader compile(std::string_view name, std::string_view version, std::string_view source,
                           std::string* out_error) const;
    CompiledShader compile_vertex_shader(std::string_view source, std::string* out_error) const;
    CompiledShader compile_geometry_shader(std::string_view source, std::string* out_error) const;
    CompiledShader compile_pixel_shader(std::string_view source, std::string* out_error) const;
    CompiledShader compile_compute_shader(std::string_view source, std::string* out_error) const;
};
} // namespace kl
