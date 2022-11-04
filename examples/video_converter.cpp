#include "klib.h"


int main(const int argc, const char** argv)
{
	std::string filepath;
	if (argc == 2) {
		filepath = argv[1];
	}
	else {
		kl::print<false>("Video file path: ");
		std::getline(std::cin, filepath);
	}

	const kl::video video(filepath);
	kl::assert(!video.is_open(), "Failed to open video file \"" + filepath + "\"");

	kl::console::clear();
	kl::print<false>("Resize the console and press enter..");
	kl::get();

	const kl::int2 console_size = kl::console::size() - kl::int2(0, 1);
	kl::console::set_cursor_enabled(false);

	const int frame_count = video.frame_count();
	std::vector<std::string> ascii_frames;
	ascii_frames.reserve(frame_count);

	kl::console::clear();
	kl::image video_frame = {};
	while (video.get_next_frame(video_frame)) {
		ascii_frames.push_back(video_frame.as_ascii(console_size));
		kl::console::move_cursor({});
		kl::print<false>("Processed: ", ascii_frames.size(), "/", frame_count);
	}

	kl::timer timer;
	kl::console::clear();
	const float to_wait = 1.0f / video.fps();

	while (true) {
		for (size_t i = 0; i < ascii_frames.size(); i++) {
			timer.reset();
			kl::console::dump(ascii_frames[i]);
			kl::console::set_title(kl::format(i + 1, "/", ascii_frames.size()));
			while (timer.get_elapsed() < to_wait);
		}
	}
}
