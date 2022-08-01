#include "window/window.h"

#include "utility/console.h"

#ifdef KL_USING_IMGUI
#include "imgui.h"
#endif


#ifdef KL_USING_IMGUI
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

LRESULT CALLBACK kl::window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_SIZE:
			resize(kl::int2(LOWORD(lParam), HIWORD(lParam)));
			break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

void kl::window::HandleMessage(const MSG& message) {
#ifdef KL_USING_IMGUI
	TranslateMessage(&message);
	if (ImGui_ImplWin32_WndProcHandler(message.hwnd, message.message, message.wParam, message.lParam)) {
		return;
	}
#endif

	switch (message.message) {
#ifdef KL_USING_IMGUI
		case WM_CHAR:
			if ((*(short*) &message.lParam) > 1) {
				ImGui::GetIO().AddInputCharacter(uint(message.wParam));
			}
			break;
#endif

		case WM_KEYDOWN:
			keys.update(message.wParam, true);
			break;

		case WM_KEYUP:
			keys.update(message.wParam, false);
			break;

		case WM_LBUTTONDOWN:
			mouse.lmb.update(kl::mouse::type::LMB, true);
			break;

		case WM_LBUTTONUP:
			mouse.lmb.update(kl::mouse::type::LMB, false);
			break;

		case WM_MBUTTONDOWN:
			mouse.mmb.update(kl::mouse::type::MMB, true);
			break;

		case WM_MBUTTONUP:
			mouse.mmb.update(kl::mouse::type::MMB, false);
			break;

		case WM_RBUTTONDOWN:
			mouse.rmb.update(kl::mouse::type::RMB, true);
			break;

		case WM_RBUTTONUP:
			mouse.rmb.update(kl::mouse::type::RMB, false);
			break;

		case WM_MOUSEMOVE:
			mouse.position(m_Window, kl::int2(GET_X_LPARAM(message.lParam), GET_Y_LPARAM(message.lParam)));
			break;

		case WM_MOUSEWHEEL:
			mouse.scroll(m_Window, GET_WHEEL_DELTA_WPARAM(message.wParam) / 120);
			break;

		default:
			DispatchMessageA(&message);
			break;
	}
}
