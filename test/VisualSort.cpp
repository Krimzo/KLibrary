#include "KrimzLib.hpp"


struct stick {
	int val = 0;
	kl::color col;
};

void renderSticks(kl::image& frameBuffer, std::vector<stick>& stickArray) {
	// Clearing the frame buffer
	frameBuffer.fillSolid(kl::colors::gray);

	// Drawing the sticks
	for (int x = 0; x < stickArray.size(); x++) {
		for (int y = (frameBuffer.getHeight() - 1 - stickArray[x].val); y < frameBuffer.getHeight(); y++) {
			frameBuffer.setPixel(kl::ivec2(x, y), stickArray[x].col);
		}
	}
}

void swapSticks(stick& a, stick& b) {
	stick t = a;
	a = b;
	b = t;
}

int main() {
	kl::window testWindow;
	kl::image frame(kl::ivec2(900, 900));

	std::vector<stick> sticks(frame.getWidth());
	for (stick& stik : sticks) {
		// Setting the value
		int randVal = 0;
		do {
			randVal = kl::random::INT(frame.getHeight() + 1);
		} while ([&]() {
			for (int i = 0; i < sticks.size(); i++) {
				if (sticks[i].val == randVal) {
					return true;
				}
			}
			return false;
			}());
		stik.val = randVal;

		// Setting the color
		const kl::byte yellowGradient = kl::random::BYTE();
		stik.col = kl::color(yellowGradient, yellowGradient, 0);
	}

	testWindow.update = [&]() {
		for (int i = 0; i <= sticks.size() / 2; i++) {
			int min = i;
			int max = sticks.size() - 1 - i;

			// Finding min
			for (int j = i + 1; j < sticks.size(); j++) {
				if (sticks[j].val < sticks[min].val) {
					min = j;
				}
			}

			// Finding max
			for (int j = i + 1; j < sticks.size() - i; j++) {
				if (sticks[j].val > sticks[max].val) {
					max = j;
				}
			}
			
			// Swapping
			swapSticks(sticks[i], sticks[min]);
			swapSticks(sticks[sticks.size() - 1 - i], sticks[max]);

			// Rendering the sticks
			renderSticks(frame, sticks);

			// Displaying the frame
			testWindow.drawImage(frame);

			// Updating title
			testWindow.setTitle(std::to_string(i + 1) + "/" + std::to_string(sticks.size()));
		}

		testWindow.setTitle("Finished!");
		testWindow.update = []() {};
	};

	testWindow.startNew(frame.getSize(), "Bubble Sort", false, true);


	return 0;
}
