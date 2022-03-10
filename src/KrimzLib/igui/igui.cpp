#include "KrimzLib/igui/igui.h"


#ifdef KL_USING_IMGUI

// Inits the ImGui context
void kl::igui::init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

// Destroys the ImGui context
void kl::igui::uninit() {
	ImGui::DestroyContext();
}

// Loads my custom theme
void kl::igui::loadKrimzTheme() {
    // Getting the gui style
    ImGuiStyle& style = ImGui::GetStyle();

    // Setting the window properties
    style.WindowPadding = ImVec2(15, 15);
    style.WindowRounding = 5.0f;
    style.FramePadding = ImVec2(5, 5);
    style.FrameRounding = 4.0f;
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 5.0f;
    style.GrabRounding = 3.0f;

    // Setting the colors
    style.Colors[ImGuiCol_Text]                 = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]         = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]             = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
    style.Colors[ImGuiCol_PopupBg]              = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    style.Colors[ImGuiCol_Border]               = ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
    style.Colors[ImGuiCol_BorderShadow]         = ImVec4(0.06f, 0.06f, 0.06f, 0.10f);
    style.Colors[ImGuiCol_FrameBg]              = ImVec4(0.10f, 0.10f, 0.10f, 0.94f);
    style.Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.70f, 0.41f, 0.02f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.70f, 0.41f, 0.02f, 0.67f);
    style.Colors[ImGuiCol_TitleBg]              = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.08f, 0.08f, 0.08f, 0.50f);
    style.Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]            = ImVec4(0.74f, 0.41f, 0.02f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]           = ImVec4(0.76f, 0.48f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.74f, 0.41f, 0.02f, 1.00f);
    style.Colors[ImGuiCol_Button]               = ImVec4(0.74f, 0.41f, 0.02f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.74f, 0.41f, 0.02f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]         = ImVec4(0.94f, 0.47f, 0.02f, 1.00f);
    style.Colors[ImGuiCol_Header]               = ImVec4(0.74f, 0.41f, 0.02f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.74f, 0.41f, 0.02f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive]         = ImVec4(0.74f, 0.41f, 0.02f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]           = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
    style.Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.74f, 0.41f, 0.02f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.74f, 0.41f, 0.02f, 0.95f);
    style.Colors[ImGuiCol_PlotLines]            = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.00f, 0.57f, 0.65f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.10f, 0.30f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.40f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.74f, 0.41f, 0.02f, 0.35f);

    // Loading fonts
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("res/fonts/Balsamiq.ttf", 15);
}

// Draws the ImGui data
void kl::igui::draw(const std::function<void()>& func) {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	func();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

#endif
