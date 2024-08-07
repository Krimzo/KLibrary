#include "examples.h"


static std::vector<float> data(100'000'000);

static void clear_data()
{
	for (auto& value : data) {
		value = 0.0f;
	}
}

static float time_it(const std::function<void()>& func)
{
	kl::Timer timer = {};
	func();
	return timer.elapsed();
}

static float compute_function(size_t value)
{
	return sin((float) value) / sqrt((float) value);
}

static void synced_test()
{
	for (size_t i = 0; i < data.size(); i++) {
		data[i] = compute_function(i);
	}
}

static void for_each_test()
{
	std::vector<size_t> indices(data.size());
	for (size_t i = 0; i < indices.size(); i++) {
		indices[i] = i;
	}
	std::for_each(std::execution::par, indices.begin(), indices.end(), [&](size_t i) {
		data[i] = compute_function(i);
	});
}

static void async_test()
{
	kl::async_for<int>(0, (int) data.size(), [&](const int i) {
		data[i] = compute_function(i);
	});
}

/* NOTE: Use Release mode for proper results. */

int examples::async_test_main(const int argc, const char** argv)
{
	static const size_t randomIndex = rand() % data.size();
	kl::print("Random index: ", randomIndex, "\n");

	clear_data();
	kl::print("synced time: ", time_it(synced_test));
	kl::print("synced data[", randomIndex, "] = ", data[randomIndex], "\n");
	
	clear_data();
	kl::print("std::for_each time: ", time_it(for_each_test));
	kl::print("std::for_each data[", randomIndex, "] = ", data[randomIndex], "\n");

	clear_data();
	kl::print("kl::async::loop time: ", time_it(async_test));
	kl::print("kl::async::loop data[", randomIndex, "] = ", data[randomIndex], "\n");

	return 0;
}
