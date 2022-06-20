#include "window/window.h"

#include "utility/console.h"

#ifdef KL_USING_IMGUI
#include "imgui.h"
#endif


void kl::window::registerWindowClass(const std::string& name) {
	WNDCLASSEXA windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		return ((kl::window*)GetWindowLongPtrA(hwnd, GWLP_USERDATA))->WndProc(hwnd, msg, wParam, lParam);
	};
	windowClass.hInstance = m_Instance;
	windowClass.lpszClassName = name.c_str();
	kl::console::error(!RegisterClassExA(&windowClass), "Failed to register window class");
}

void kl::window::createWindow(const kl::uint2& size, const std::string& name, bool resizeable) {
	m_WindowStyle = resizeable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	RECT adjustedWindowSize = { 0, 0, LONG(size.x), LONG(size.y) };
	AdjustWindowRect(&adjustedWindowSize, m_WindowStyle, false);
	const kl::int2 adjSize(adjustedWindowSize.right - adjustedWindowSize.left, adjustedWindowSize.bottom - adjustedWindowSize.top);

	m_Window = CreateWindowExA(0, name.c_str(), name.c_str(), m_WindowStyle, (kl::screen::size.x / 2 - adjSize.x / 2), (kl::screen::size.y / 2 - adjSize.y / 2), adjSize.x, adjSize.y, nullptr, nullptr, m_Instance, nullptr);
	kl::console::error(!m_Window, "Failed to create window");

	SetWindowLongPtrA(m_Window, GWLP_USERDATA, int64(this));
	ShowWindow(m_Window, SW_SHOW);
	m_DeviceContext = GetDC(m_Window);
	m_WindowStyle = GetWindowLongA(m_Window, GWL_STYLE);
}

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

void kl::window::handleMessage(MSG& msg) {
#ifdef KL_USING_IMGUI
	TranslateMessage(&msg);
	if (ImGui_ImplWin32_WndProcHandler(msg.hwnd, msg.message, msg.wParam, msg.lParam)) {
		return;
	}
#endif

	// Default
	switch (msg.message) {
#ifdef KL_USING_IMGUI
	case WM_CHAR:
		if (*(short*)&msg.lParam > 1) {
			ImGui::GetIO().AddInputCharacter(uint(msg.wParam));
		}
		break;
#endif

	case WM_KEYDOWN:
		keys.update(msg.wParam, true);
		break;

	case WM_KEYUP:
		keys.update(msg.wParam, false);
		break;

	case WM_LBUTTONDOWN:
		mouse.lmb.update(true);
		break;

	case WM_LBUTTONUP:
		mouse.lmb.update(false);
		break;

	case WM_MBUTTONDOWN:
		mouse.mmb.update(true);
		break;

	case WM_MBUTTONUP:
		mouse.mmb.update(false);
		break;

	case WM_RBUTTONDOWN:
		mouse.rmb.update(true);
		break;

	case WM_RBUTTONUP:
		mouse.rmb.update(false);
		break;

	case WM_MOUSEMOVE:
		mouse.position = kl::int2(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));
		break;

	case WM_MOUSEWHEEL:
		mouse.scroll += GET_WHEEL_DELTA_WPARAM(msg.wParam) / 120;
		break;

	default:
		DispatchMessageA(&msg);
		break;
	}
}
