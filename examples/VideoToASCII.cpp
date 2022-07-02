#include "KrimzLib.h"


int main(int argc, char** argv) {
	std::string filePath;
	if (argc == 2) {
		filePath = argv[1];
	}
	else {
		kl::print<kl::none>("Video file path: ");
		std::getline(std::cin, filePath);
	}

	kl::video video(filePath);
	kl::console::error(!video.isOpen(), "Failed to open video file \"" + filePath + "\"");

	kl::console::clear();
	kl::print<kl::none>("Resize the console and press enter..");
	kl::console::waitFor(kl::enter);
	const kl::int2 consoleSize = kl::console::size() - kl::int2(0, 1);
	kl::console::hideCursor();

	kl::console::clear();
	kl::image videoFrame;
	const uint64 frameCount = video.frameCount();
	std::vector<std::string> asciiFrames;
	asciiFrames.reserve(frameCount);
	while (video.getFrame(videoFrame)) {
		asciiFrames.push_back(videoFrame.ascii(consoleSize));
		kl::console::moveCursor({});
		kl::print<kl::none>("Processed: ", asciiFrames.size(), "/", frameCount);
	}

	kl::timer timer;
	kl::console::clear();
	const double toWait = video.frameTime();
	while (true) {
		for (uint64 i = 0; i < asciiFrames.size(); i++) {
			timer.reset();
			kl::console::dump(asciiFrames[i]);
			kl::console::title(kl::format(i + 1, "/", asciiFrames.size()));
			while (timer.elapsed() < toWait);
		}
	}
}
