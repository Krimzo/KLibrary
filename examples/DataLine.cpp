#include "KrimzLib.h"


void DrawAxis(kl::image* frame, const kl::color& col = kl::colors::lgray) {
	frame->drawLine(kl::int2(0, frame->height() / 2), kl::int2(frame->width(), frame->height() / 2), col);
	frame->drawLine(kl::int2(frame->width() / 2, 0), kl::int2(frame->width() / 2, frame->height()), col);
}
void DrawData(kl::image* frame, std::vector<kl::int2>& data, const kl::color& col = kl::colors::orange) {
	const kl::uint2 halfSize = frame->size() / 2;
	for (auto& val : data) {
		frame->pixel(val * kl::int2(1, -1) + halfSize, col);
	}
}
void DrawLine(kl::image* frame, const kl::float2& equat, const kl::color& col = kl::colors::sky) {
	const kl::int2 halfSize = frame->size() / 2;
	const kl::int2 pos1 = kl::int2(-halfSize.x, int(-halfSize.x * equat.x + equat.y)) * kl::int2(1, -1) + halfSize;
	const kl::int2 pos2 = kl::int2(halfSize.x, int(halfSize.x * equat.x + equat.y)) * kl::int2(1, -1) + halfSize;
	frame->drawLine(pos1, pos2, col);
}

float CalcOffsets(const std::vector<kl::int2>& data, const kl::float2& lineEquat) {
	float sum = 0.0f;
	for (auto& val : data) {
		sum += abs(val.y - (val.x * lineEquat.x + lineEquat.y));
	}
	return sum;
}
void DrawOffset(kl::image* frame, const std::vector<kl::int2>& data, const kl::float2& lineEquat, const kl::color& col = kl::colors::yellow) {
	static size_t dataIndex = 0;
	if (data.size() > 0) {
		dataIndex = (dataIndex + 1) % data.size();
		const kl::int2 halfSize = frame->size() / 2;
		const kl::int2 pos1 = kl::int2(data[dataIndex].x, int(data[dataIndex].y)) * kl::int2(1, -1) + halfSize;
		const kl::int2 pos2 = kl::int2(data[dataIndex].x, int(data[dataIndex].x * lineEquat.x + lineEquat.y)) * kl::int2(1, -1) + halfSize;
		frame->drawLine(pos1, pos2, col);
	}
}

void CalcBetterLine(const std::vector<kl::int2>& data, kl::float2& lineEquat) {
	static size_t lastDataSize = 0;
	static kl::float2 alterX(10.0f, 0.0f);
	static kl::float2 alterY(0.0f, 10.0f);

	if (data.size() != lastDataSize) {
		lastDataSize = data.size();
		alterX = { 10.0f, 0.0f };
		alterY = { 0.0f, 10.0f };
		lineEquat = { 1.0f, 0.0f };
	}

	const float origSum = CalcOffsets(data, lineEquat);

	if (CalcOffsets(data, lineEquat + alterX) < origSum) {
		lineEquat += alterX;
	}
	else if (CalcOffsets(data, lineEquat - alterX) < origSum) {
		lineEquat -= alterX;
	}
	else {
		alterX *= 0.75f;
	}

	if (CalcOffsets(data, lineEquat + alterY) < origSum) {
		lineEquat += alterY;
	}
	else if (CalcOffsets(data, lineEquat - alterY) < origSum) {
		lineEquat -= alterY;
	}
	else {
		alterY *= 0.75f;
	}
}

int main() {
	kl::console::hide();

	kl::window win;
	kl::double_buffer buff({ 1600, 900 });

	win.update = [&]() {
		buff >> win;
	};

	win.end = [&]() {
		exit(0);
	};

	std::thread([&]() {
		win.run(buff.size(), "Data Display", false, true);
	}).detach();

	std::vector<kl::int2> data;
	kl::float2 lineEquat(1.0f, 0.0f);

	win.mouse.lmb.down = [&]() {
		static kl::int2 lastData = {};
		const kl::int2 newData = win.mouse.position * kl::int2(1, -1) - kl::int2(buff.size()) / kl::int2(2, -2);
		if (newData != lastData) {
			data.push_back(newData);
			lastData = newData;
		}
	};
	win.keys.r.press = [&]() {
		data.clear();
	};

	while (true) {
		win.title(kl::format("FPS: ", int(1.0f / kl::time::interval())));

		kl::image* bb = buff.getBB();
		bb->fill(kl::colors::gray);

		DrawAxis(bb);
		DrawData(bb, data);
		DrawLine(bb, lineEquat);
		DrawOffset(bb, data, lineEquat);
		CalcBetterLine(data, lineEquat);

		buff.swap();
	}
}
