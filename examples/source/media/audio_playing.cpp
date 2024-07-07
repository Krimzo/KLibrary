#include "examples.h"


int examples::audio_playing_main()
{
	kl::AudioDevice device{ 0 };
	kl::Audio audio{ "media/chicken.wav" };
	device.play_audio(audio);

	kl::print("Say something!");
	audio.set_duration(2.5f);
	device.record_audio(&audio);

	kl::print("Now listen:");
	device.play_audio(audio);
	return 0;
}
