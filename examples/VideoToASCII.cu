#include "KrimzLib.h"


int main(int argc, char** argv) {
	// Arg check
	if (argc < 2) {
		std::cout << "Mising video file arguments!" << std::endl;
		return 1;
	}

	// Opening video
	const std::string filePath = argv[1];
	kl::video video(filePath);
	if (!video.isOpen()) {
		std::cout << "Video file could not be opened!" << std::endl;
		return 2;
	}

	// Waiting for user
	kl::console::clear();
	std::cout << "Resize the console and press enter..";
	std::cin.get();
	const kl::int2 consoleSize = kl::console::size() - kl::int2(0, 1);
	kl::console::hideCursor();

	// Processing frames
	kl::console::clear();
	std::vector<std::string> asciiFrames;
	kl::image videoFrame;
	while (video.getFrame(videoFrame)) {
		static int counter = 0;
		asciiFrames.push_back(videoFrame.ascii(consoleSize));
		kl::console::moveCursor(0);
		std::cout << "Processed " << ++counter << " frames!";
	}

	// Displaying frames
	kl::console::clear();
	kl::timer timer;
	while (true) {
		static int counter = 0;
		kl::console::fastOut(asciiFrames[counter++ % asciiFrames.size()]);

		static const float toWait = 1.0f / video.frameRate();
		while (timer.elapsed() < toWait);
		kl::console::setTitle("Frame time: " + std::to_string(timer.elapsed()));
		timer.reset();
	}
}
