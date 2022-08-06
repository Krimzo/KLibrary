#include "window/window.h"

#include "utility/console.h"

#ifdef KL_USING_IMGUI
#include "imgui.h"
#endif


#ifdef KL_USING_IMGUI
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

LRESULT CALLBACK kl::Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_SIZE) {
		onResize({ LOWORD(lParam), HIWORD(lParam) });
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

void kl::Window::HandleMessage(const MSG& message) {
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
			keyboard.updateValue(message.wParam, true);
			break;

		case WM_KEYUP:
			keyboard.updateValue(message.wParam, false);
			break;

		case WM_LBUTTONDOWN:
			mouse.left.updateValue(Mouse::Type::LMB, true);
			break;

		case WM_LBUTTONUP:
			mouse.left.updateValue(Mouse::Type::LMB, false);
			break;

		case WM_MBUTTONDOWN:
			mouse.middle.updateValue(Mouse::Type::MMB, true);
			break;

		case WM_MBUTTONUP:
			mouse.middle.updateValue(Mouse::Type::MMB, false);
			break;

		case WM_RBUTTONDOWN:
			mouse.right.updateValue(Mouse::Type::RMB, true);
			break;

		case WM_RBUTTONUP:
			mouse.right.updateValue(Mouse::Type::RMB, false);
			break;

		case WM_MOUSEMOVE:
			mouse.updatePosition(m_Window, { GET_X_LPARAM(message.lParam), GET_Y_LPARAM(message.lParam) });
			break;

		case WM_MOUSEWHEEL:
			mouse.updateScroll(m_Window, GET_WHEEL_DELTA_WPARAM(message.wParam) / 120);
			break;

		default:
			DispatchMessageA(&message);
			break;
	}
}
