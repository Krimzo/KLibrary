#pragma once

#include <functional>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"


namespace kl {
	namespace igui {
		// Inits the ImGui context
		void init();

		// Destroys the ImGui context
		void uninit();

		// Loads my custom theme
		void loadKrimzTheme();

		// Draws the ImGui data
		void draw(const std::function<void()>& func);
	}
}
