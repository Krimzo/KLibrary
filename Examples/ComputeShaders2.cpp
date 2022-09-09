#include "KrimzLib.h"

using namespace kl;


struct ExampleStruct {
	Float3 position;
};

int main() {
	constexpr int dataSize = 10;
	ExampleStruct exampleData[dataSize] = {};

	GPU gpu = {};

	dx::Buffer buffer = gpu.newStructuredBuffer(exampleData, dataSize, sizeof(ExampleStruct), true, true);
	dx::AccessView shaderView = gpu.newAccessView(buffer);

	dx::ComputeShader computeShader = gpu.newComputeShader(Files::ReadString("Examples/Shaders/ComputeTest2.hlsl"));
	gpu.bindComputeAccessView(shaderView, 0);
	gpu.executeComputeShader(computeShader, { dataSize, 1, 1 });

	gpu.readFromResource(exampleData, buffer, sizeof(exampleData));

	for (auto& example : exampleData) {
		Print(example.position);
	}


	Get();
}
