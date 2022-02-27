#include "KrimzLib/KrimzLib.hpp"

#include <thread>


// Multithreaded for loop
void ParallelFor(int startInclusive, int endExclusive, std::function<void(int)> loopBody) {
	// Thread storage
	std::thread cpuThreads[4];

	// Thread creation
	const int runsPerThread = (endExclusive - startInclusive) / 4;
	for (int i = 0; i < 4; i++) {
		const int loopStart = runsPerThread * i + startInclusive;
		const int loopEnd = (i == 3) ? endExclusive : (loopStart + runsPerThread);
		cpuThreads[i] = std::thread([&](int start, int end) { for (int i = start; i < end; i++) { loopBody(i); } }, loopStart, loopEnd);
	}

	// Waiting for the threads to finish
	for (int i = 0; i < 4; i++) {
		cpuThreads[i].join();
	}
}

int main() {
	kl::window win;
	kl::image frame(kl::ivec2(1600, 900));
	kl::timer timer;
	kl::camera camera;

	std::vector<kl::vertex> cubeVertices = kl::file::parseObj("res/objects/cube.obj");

	win.update = [&]() {
		// Time
		const float deltaT = timer.interval();

		// Inverse camera matrix calc
		camera.position.z = -3.0f;
		const kl::mat4 invCam = camera.matrix().inverse();

		// Raytracing
		const int trianCount = int(cubeVertices.size()) / 3;
		ParallelFor(0, frame.getHeight(), [&](int y) {
			for (int x = 0; x < frame.getWidth(); x++) {
				// Pixel can be cleared
				bool canClear = true;

				// Pixel depth
				float pixelDepth = INFINITY;

				// Triangle loop
				for (int i = 0; i < trianCount; i++) {
					// Ndc calc
					kl::vec2 ndc = kl::vec2(float(x), float(y)) / kl::vec2(frame.getSize());
					ndc *= kl::vec2(2.0f);
					ndc -= kl::vec2(1.0f);

					// Ray direction calc
					kl::vec4 rayDir = invCam * kl::vec4(ndc, 1.0f, 1.0f);
					rayDir /= rayDir.w;

					// Ray creation
					kl::ray ray(camera.position, rayDir.xyz());

					// Triangle creation
					kl::triangle triangle(
						cubeVertices[i * 3 + 0],
						cubeVertices[i * 3 + 1],
						cubeVertices[i * 3 + 2]
					);

					// Intersection test
					kl::vec3 intersection;
					if (ray.intersect(triangle, &intersection)) {
						// Clear switch
						canClear = false;

						// Depth calc
						const float tempDepth = (intersection - camera.position).length();

						// Depth check
						if (tempDepth < pixelDepth) {
							// Depth save
							pixelDepth = tempDepth;

							// Pixel draw
							frame.setPixel(kl::ivec2(x, y), kl::colors::wheat);
						}
					}
					else if (canClear) {
						// Background draw
						frame.setPixel(kl::ivec2(x, y), kl::colors::gray);
					}
				}
			}
		});

		// Displaying the frame
		win.drawImage(frame);

		// Fps display
		win.setTitle(std::to_string(int(1 / deltaT)));
	};

	timer.interval();
	win.startNew(frame.getSize(), "xD", false, true);
}
