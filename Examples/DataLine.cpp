#include "KrimzLib.h"


static void DrawAxis(kl::Image& frame, const kl::Color& color = { 75, 75, 75 }) {
	frame.drawLine({ 0, frame.getHeight() / 2 }, { frame.getWidth(), frame.getHeight() / 2 }, color);
	frame.drawLine({ frame.getWidth() / 2, 0 }, { frame.getWidth() / 2, frame.getHeight() }, color);
}

static void DrawData(kl::Image& frame, kl::Vector<kl::Int2>& data, const kl::Color& color = kl::Colors::Orange) {
	const kl::UInt2 halfSize = frame.getSize() / 2;
	for (auto& val : data) {
		frame.setPixel(val * kl::Int2(1, -1) + halfSize, color);
	}
}

static void DrawLine(kl::Image& frame, const kl::Float2& equat, const kl::Color& color = kl::Colors::Sky) {
	const kl::Int2 halfSize = frame.getSize() / 2;
	const kl::Int2 pos1 = kl::Int2(-halfSize.x, int(-halfSize.x * equat.x + equat.y)) * kl::Int2(1, -1) + halfSize;
	const kl::Int2 pos2 = kl::Int2(halfSize.x, int(halfSize.x * equat.x + equat.y)) * kl::Int2(1, -1) + halfSize;
	frame.drawLine(pos1, pos2, color);
}

static float CalculateOffsets(const kl::Vector<kl::Int2>& data, const kl::Float2& lineEquat) {
	float sum = 0.0f;
	for (auto& val : data) {
		sum += abs(val.y - (val.x * lineEquat.x + lineEquat.y));
	}
	return sum;
}

static void DrawOffset(kl::Image& frame, const kl::Vector<kl::Int2>& data, const kl::Float2& lineEquat, const kl::Color& color = kl::Colors::Yellow) {
	static size_t dataIndex = 0;
	if (data.size() > 0) {
		dataIndex = (dataIndex + 1) % data.size();
		const kl::Int2 halfSize = frame.getSize() / 2;
		const kl::Int2 pos1 = kl::Int2(data[dataIndex].x, int(data[dataIndex].y)) * kl::Int2(1, -1) + halfSize;
		const kl::Int2 pos2 = kl::Int2(data[dataIndex].x, int(data[dataIndex].x * lineEquat.x + lineEquat.y)) * kl::Int2(1, -1) + halfSize;
		frame.drawLine(pos1, pos2, color);
	}
}

static void CalculateImprovedLine(const kl::Vector<kl::Int2>& data, kl::Float2& lineEquat) {
	static kl::uint64 lastDataSize = 0;
	static kl::Float2 alterX(10.0f, 0.0f);
	static kl::Float2 alterY(0.0f, 10.0f);

	if (data.size() != lastDataSize) {
		lastDataSize = data.size();
		alterX = { 10.0f, 0.0f };
		alterY = { 0.0f, 10.0f };
		lineEquat = { 1.0f, 0.0f };
	}

	const float origSum = CalculateOffsets(data, lineEquat);

	if (CalculateOffsets(data, lineEquat + alterX) < origSum) {
		lineEquat += alterX;
	}
	else if (CalculateOffsets(data, lineEquat - alterX) < origSum) {
		lineEquat -= alterX;
	}
	else {
		alterX *= 0.75f;
	}

	if (CalculateOffsets(data, lineEquat + alterY) < origSum) {
		lineEquat += alterY;
	}
	else if (CalculateOffsets(data, lineEquat - alterY) < origSum) {
		lineEquat -= alterY;
	}
	else {
		alterY *= 0.75f;
	}
}

int main() {
	kl::Window window = { { 1600, 900 }, "Data Line" };
	kl::Image frame = { window.getSize() };

	kl::Vector<kl::Int2> data;
	kl::Float2 lineEquat(1.0f, 0.0f);

	window.mouse.left.onDown = [&]() {
		static kl::Int2 lastData = {};
		kl::Int2 newData = window.mouse.getPosition() * kl::Int2(1, -1);
		newData -= kl::Int2(frame.getSize()) / kl::Int2(2, -2);
		if (newData != lastData) {
			data.push_back(newData);
			lastData = newData;
		}
	};

	window.keyboard.r.onPress = [&]() {
		data.clear();
	};

	while (window.process(false)) {
		frame.fill(kl::Colors::Gray);

		DrawAxis(frame);
		DrawData(frame, data);
		DrawLine(frame, lineEquat);
		DrawOffset(frame, data, lineEquat);

		CalculateImprovedLine(data, lineEquat);

		window.drawImage(frame);
	}
}
