#pragma once

#if defined(__has_include) && __has_include(<imgui.h>)
#define KL_USING_IMGUI
#endif

#ifdef KL_USING_IMGUI

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

namespace imgui = ImGui;

#endif
