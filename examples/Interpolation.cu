#include "KrimzLib.h"


const uint frameSize = 900;

struct Point : public kl::float2 {
	kl::color color = kl::colors::black;

	Point() : kl::float2() {}
	Point(float x, float y, const kl::color& color) : kl::float2(x, y), color(color) {}
};

RUN void InterpolationKernel(uint64 n, kl::color* outBuffer, kl::triangle T1, kl::triangle T2, kl::color colA, kl::color colB, kl::color colC, kl::color colD) {
	const uint64 i = kl::cuda::index();
	if (i < n) {
		const kl::uint2 pixelPos = { i % frameSize, i / frameSize };

		const kl::float3 t1Weights = T1.weights(T1.constants(), pixelPos);
		const kl::float3 t2Weights = T2.weights(T2.constants(), pixelPos);

		kl::color pixelCol = { 50, 50, 50 };
		if (T1.in(t1Weights)) {
			pixelCol.r = byte(T1.interpolate(t1Weights, kl::float3(colA.r, colB.r, colC.r)));
			pixelCol.g = byte(T1.interpolate(t1Weights, kl::float3(colA.g, colB.g, colC.g)));
			pixelCol.b = byte(T1.interpolate(t1Weights, kl::float3(colA.b, colB.b, colC.b)));
		}
		else if (T2.in(t2Weights)) {
			pixelCol.r = byte(T2.interpolate(t2Weights, kl::float3(colA.r, colD.r, colC.r)));
			pixelCol.g = byte(T2.interpolate(t2Weights, kl::float3(colA.g, colD.g, colC.g)));
			pixelCol.b = byte(T2.interpolate(t2Weights, kl::float3(colA.b, colD.b, colC.b)));
		}
		outBuffer[i] = pixelCol;
	}
}
static kl::cuda::kernel interpolationKernel = InterpolationKernel;


int main() {
	kl::window window;

	Point points[4] = {
		Point(50.0f, (frameSize - 1.0f) / 3.0f, kl::colors::red),
		Point((frameSize - 1.0f) / 3.0f, frameSize - 51.0f, kl::colors::green),
		Point(frameSize - 51.0f, (frameSize - 1.0f) * 0.667f, kl::colors::blue),
		Point((frameSize - 1.0f) * 0.667f, 50.0f, kl::colors::white)
	};

	kl::color* frameBuffer = nullptr;
	window.start = [&]() {
		kl::cuda::alloc(frameBuffer, frameSize * frameSize);
	};

	kl::image frameImage(frameSize);
	Point* pointToMove = nullptr;
	window.update = [&]() {
		interpolationKernel.runs = frameSize * frameSize;
		interpolationKernel.run(interpolationKernel.runs, frameBuffer,
			kl::triangle(kl::vertex(kl::float3(points[0], 0.5f)), kl::vertex(kl::float3(points[1], 0.5f)), kl::vertex(kl::float3(points[2], 0.5f))),
			kl::triangle(kl::vertex(kl::float3(points[0], 0.5f)), kl::vertex(kl::float3(points[3], 0.5f)), kl::vertex(kl::float3(points[2], 0.5f))),
			points[0].color, points[1].color, points[2].color, points[3].color);

		kl::cuda::copy(frameImage.data(), frameBuffer, frameSize * frameSize, kl::cuda::transfer::DH);

		float minDis = FLT_MAX;
		for (auto& point : points) {
			float dis = (window.mouse.position - point).length();
			if (dis < minDis) {
				minDis = dis;
				pointToMove = &point;
			}
		}
		frameImage.drawCircle(*pointToMove, window.mouse.position, pointToMove->color);

		window.draw(frameImage);
	};
	window.mouse.lmb.down = [&]() {
		pointToMove->x = window.mouse.position.x;
		pointToMove->y = window.mouse.position.y;
	};

	window.end = [&]() {
		kl::cuda::free(frameBuffer);
	};

	window.run(frameSize, "Triangle Interpolation", false, false);
}
