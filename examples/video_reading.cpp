#include "klib.h"


int main(const int argc, const char** argv)
{
	kl::media_utility::init();

	std::string filepath = {};
	if (argc == 2) {
		filepath = argv[1];
	}
	else {
		kl::print<false>("Video file path: ");
		std::getline(std::cin, filepath);
	}

	auto video_reader = kl::make<kl::video_reader>(filepath);

	kl::console::clear();
	kl::print<false>("Resize the console and press enter..");
	kl::get();

	const kl::int2 console_size = kl::console::size() - kl::int2(0, 1);
	kl::console::set_cursor_enabled(false);

	const int frame_count = video_reader->frame_count();
	std::vector<std::string> ascii_frames;
	ascii_frames.reserve(frame_count);

	kl::console::clear();
	kl::image video_frame = {};
	while (video_reader->get_next_frame(video_frame)) {
		ascii_frames.push_back(video_frame.as_ascii(console_size));
		kl::console::move_cursor({});
		kl::print<false>("Processed: ", ascii_frames.size(), "/", frame_count);
	}

	kl::timer timer = {};
	kl::console::clear();
	const float to_wait = 1.0f / video_reader->fps();

	while (true) {
		for (size_t i = 0; i < ascii_frames.size(); i++) {
			timer.reset();
			kl::console::dump(ascii_frames[i]);
			kl::console::set_title(kl::format(i + 1, "/", ascii_frames.size()));
			while (timer.get_elapsed() < to_wait);
		}
	}

	video_reader = nullptr; // Has to be closed before kl::media_utility::uninit()

	kl::media_utility::uninit();
}
