#include "KrimzLib.h"

using namespace kl;


struct CS_CB {
	Float4 miscData;
};

static UInt3 CalculateWindowDispatch(const Window& window) {
	UInt3 result = {};
	result.x = (window.getWidth() / 32) + ((window.getWidth() % 32) ? 1 : 0);
	result.y = (window.getHeight() / 32) + ((window.getHeight() % 32) ? 1 : 0);
	result.z = 1;
	return result;
}

int main() {
	Window window = { { 1600, 900 }, "Testing" };
	GPU gpu = GPU(window);
	Timer timer = {};

	window.setResizeable(false);

	gpu.bindDepthState(gpu.newDepthState(false, false, false));

	dx::Texture renderTexture = gpu.newTexture(Image({ 1600, 900 }, kl::Colors::Gray), true, true);
	dx::ShaderView pixelShaderView = gpu.newShaderView(renderTexture);
	dx::AccessView computeShaderView = gpu.newAccessView(renderTexture);
	dx::TargetView targetView = gpu.newTargetView(renderTexture);

	String shadersSource = Files::ReadString("Examples/Shaders/ComputeTest.hlsl");
	Shaders defaultShaders = gpu.newShaders(shadersSource);
	dx::ComputeShader computeShader = gpu.newComputeShader(shadersSource);

	dx::Buffer screenMesh = gpu.newVertexBuffer({
		{ { -1.0f, -1.0f, 0.5f } }, { { -1.0f, 1.0f, 0.5f } }, { { 1.0f, 1.0f, 0.5f } },
		{ { 1.0f, 1.0f, 0.5f } }, { { 1.0f, -1.0f, 0.5f } }, { { -1.0f, -1.0f, 0.5f } } });

	while (window.process(false)) {
		timer.updateInterval();

		gpu.bindComputeShaderView(nullptr, 0);
		gpu.bindPixelShaderView(nullptr, 0);
		gpu.clearTargetView(targetView, kl::Colors::Gray);

		CS_CB csData = {};
		csData.miscData.x = (float)window.mouse.getPosition().x;
		csData.miscData.y = (float)window.mouse.getPosition().y;
		gpu.autoComputeCBuffer(csData);

		gpu.bindComputeShader(computeShader);
		gpu.bindPixelShaderView(nullptr, 0);
		gpu.bindComputeAccessView(computeShaderView, 0);
		gpu.dispatchComputeShader(CalculateWindowDispatch(window));

		gpu.bindShaders(defaultShaders);
		gpu.bindComputeAccessView(nullptr, 0);
		gpu.bindPixelShaderView(pixelShaderView, 0);
		gpu.drawVertexBuffer(screenMesh);

		gpu.swapBuffers(true);

		window.setTitle(Format(int(1.0f / timer.getInterval())));
	}
}
