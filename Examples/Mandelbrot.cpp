#include "KrimzLib.h"

using namespace kl;


struct PS_CB {
	Float4 stateInfo;
	Float4 frameSize;
	Float4 startColor;
};

static int iterations = 64;
static float zoom = 1.0f;
static Float2 position = { -0.5f, 0.0f };
static Color startColor = Colors::Orange;

static void Input(Window& window, float deltaTime) {
	// Keyboard
	if (window.keyboard.esc) {
		window.close();
	}
	if (window.keyboard.r) {
		if (window.keyboard.shift) {
			iterations = 64;
		}
		else {
			position = { -0.5, 0.0f };
			zoom = 1.0f;
		}
	}
	if (window.keyboard.w) {
		position.y -= (1.0f / zoom) * deltaTime;
	}
	if (window.keyboard.s) {
		position.y += (1.0f / zoom) * deltaTime;
	}
	if (window.keyboard.d) {
		position.x += (1.0f / zoom) * deltaTime;
	}
	if (window.keyboard.a) {
		position.x -= (1.0f / zoom) * deltaTime;
	}

	// Mouse
	Int2 frameSize = window.getSize();
	if (window.mouse.left) {
		zoom += zoom * deltaTime;
		Float2 uv = ((Float2(window.mouse.getPosition()) / frameSize) * 2.0f - Float2(1.0f, 1.0f)) * (float(frameSize.x) / frameSize.y);
		position += (uv / zoom) * deltaTime;
	}
	if (window.mouse.right) {
		zoom -= zoom * deltaTime;
		Float2 uv = ((Float2(window.mouse.getPosition()) / frameSize) * 2.0f - Float2(1.0f, 1.0f)) * (float(frameSize.x) / frameSize.y);
		position -= (uv / zoom) * deltaTime;
	}

	// Scroll
	static int lastScroll = window.mouse.getScroll();
	int currentScroll = window.mouse.getScroll();
	iterations += (currentScroll - lastScroll) * (window.keyboard.shift ? 10 : 1);
	lastScroll = currentScroll;
}

static void ConsoleRead() {
	while (true) {
		Print<false>(Colors::Default, "Color = ");
		Vector<String> parts = Strings::Split([]() {
			String line; std::getline(std::cin, line); return line;
		}(), ' ');

		if (parts.size() >= 3) {
			try {
				Color resultColor = {};
				resultColor.r = (byte)std::stoi(parts[0]);
				resultColor.g = (byte)std::stoi(parts[1]);
				resultColor.b = (byte)std::stoi(parts[2]);
				startColor = resultColor;
				Print(startColor, "Color updated!");
			}
			catch (std::exception e) {
				Print(Colors::Red, "Bad input");
			}
		}
		else {
			Print(Colors::Orange, "Not enough data");
		}
	}
}

int main() {
	Window window = { { 1600, 900 }, "Mandelbrot" };
	GPU gpu = GPU(window);
	Timer timer;

	window.onResize = [&](Int2 newSize) {
		if (newSize.x > 0 && newSize.y > 0) {
			gpu.resizeInternal(newSize);
			gpu.setViewport(newSize);
		}
	};
	window.maximize();

	// Start
	gpu.bindShaders(gpu.newShaders(Files::ReadString("Examples/Shaders/Mandelbrot.hlsl")));
	dx::Buffer screenMesh = gpu.generateScreenMesh();

	// Console
	std::thread(ConsoleRead).detach();

	// Update
	while (window.process(false)) {
		timer.updateInterval();

		// Input
		Input(window, timer.getInterval());

		// Render
		PS_CB psData = {};
		psData.stateInfo.xy = position;
		psData.stateInfo.z = zoom;
		psData.stateInfo.w = (float)iterations;
		psData.frameSize.xy = window.getSize();
		psData.startColor = startColor;

		gpu.clearInternal();
		gpu.autoPixelCBuffer(psData);
		gpu.drawVertexBuffer(screenMesh);
		gpu.swapBuffers(true);

		// Info
		window.setTitle(Format(
			"[", std::setw(3), int(1 / timer.getInterval()), "] ",
			"(Iterations: ", iterations, ") ",
			"(Zoom: ", std::fixed, std::setprecision(2), zoom, ") ",
			"(Position: ", position, ")"
		));
	}
}
