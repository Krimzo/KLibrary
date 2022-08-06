#include "KrimzLib.h"


int main(int argc, const char** argv) {
	kl::String filePath;
	if (argc == 2) {
		filePath = argv[1];
	}
	else {
		kl::Print<false>("Video file path: ");
		std::getline(std::cin, filePath);
	}

	kl::Video video(filePath);
	kl::Assert(!video, "Failed to open video file \"" + filePath + "\"");

	kl::Console::Clear();
	kl::Print<false>("Resize the console and press enter..");
	kl::Get();

	const kl::Int2 consoleSize = kl::Console::GetSize() - kl::Int2(0, 1);
	kl::Console::SetCursorEnabled(false);

	const kl::uint64 frameCount = video.frameCount();
	kl::Vector<kl::String> asciiFrames;
	asciiFrames.reserve(frameCount);

	kl::Image videoFrame;
	kl::Console::Clear();
	while (video.getFrame(videoFrame)) {
		asciiFrames.push_back(videoFrame.asASCII(consoleSize));
		kl::Console::MoveCursor({});
		kl::Print<false>("Processed: ", asciiFrames.size(), "/", frameCount);
	}

	kl::Timer timer;
	kl::Console::Clear();
	float toWait = 1.0f / video.fps();

	while (true) {
		for (kl::uint64 i = 0; i < asciiFrames.size(); i++) {
			timer.reset();
			kl::Console::DumpData(asciiFrames[i]);
			kl::Console::SetTitle(kl::Format(i + 1, "/", asciiFrames.size()));
			while (timer.getElapsed() < toWait);
		}
	}
}
