#include "KrimzLib.h"

using namespace kl;


struct Object {
	float radius = Random::FLOAT(10.0f, 20.0f);
	Float2 position = {};
	Float2 velocity = {};
	Color color = Random::COLOR();
	Vector<Float2> positionHistory = {};
};

static float GetFrameLowerLimit(const Image& frame) {
	return frame.getHeight() * 0.9f;
}

static void DrawBackground(Image& frame) {
	float lowerLimit = GetFrameLowerLimit(frame);
	frame.fill({ 60, 60, 60 });
	frame.drawRectangle({ 0, lowerLimit }, frame.getSize(), Colors::Gray, true);
	frame.drawLine({ 0, lowerLimit }, { frame.getWidth(), lowerLimit }, { 30, 30, 30 });
}

static void DrawObjects(Image& frame, Vector<Object>& objects) {
	for (auto& object : objects) {
		object.positionHistory.push_back(object.position);
		int startPositionIndex = int(object.positionHistory.size() * 0.75f);
		for (int i = startPositionIndex; i < object.positionHistory.size() - 1; i++) {
			frame.drawLine(object.positionHistory[i], object.positionHistory[i + 1ULL],
				Color(60, 60, 60).mix(object.color, Math::Normalize(i, startPositionIndex, object.positionHistory.size())));
		}
		frame.drawCircle(object.position, object.radius, object.color, true);
		frame.drawCircle(object.position, object.radius, { 30, 30, 30 });
	}
}

static void DrawInterface(Image& frame, bool drawArrow, Float2 arrowStart, Float2 arrowEnd) {
	if (drawArrow) {
		for (int i = -1; i <= 1; i++) {
			const Float2 adder = { i, i };
			frame.drawLine(arrowStart + adder, arrowEnd + adder, Colors::Sky);
		}
	}
}

static void ProcessObjects(Vector<Object>& objects, const Timer& timer, const Image& frame, const Float2& gravity) {
	static const float energyRetain = 0.8f;
	
	for (auto& object : objects) {
		object.velocity += gravity * timer.getInterval();
		object.position += object.velocity * timer.getInterval();
	}

	for (int i = 0; i < objects.size(); i++) {
		int intersectCount = 0;
		Float2 velocitySum, positionSum;
		for (int j = 0; j < objects.size(); j++) {
			if (i != j) {
				Float2 positionsVector = objects[i].position - objects[j].position;
				float radiusSum = objects[i].radius + objects[j].radius;
				if (positionsVector.length() < radiusSum) {
					positionSum += objects[j].position + positionsVector.normalize() * radiusSum;
					velocitySum += objects[i].velocity.reflect(objects[j].position - objects[i].position);
					intersectCount += 1;
				}
			}
		}
		if (intersectCount > 0) {
			float countRec = 1.0f / intersectCount;
			objects[i].position = positionSum * countRec;
			objects[i].velocity = velocitySum * countRec * energyRetain;
		}
	}

	for (auto& object : objects) {
		if (object.position.x - object.radius < 0) {
			object.position.x = object.radius;
			object.velocity.x *= -1.0f;
			object.velocity *= energyRetain;
		}
		if (object.position.x + object.radius > frame.getWidth()) {
			object.position.x = frame.getWidth() - object.radius;
			object.velocity.x *= -1.0f;
			object.velocity *= energyRetain;
		}
		if (object.position.y - object.radius < 0) {
			object.position.y = object.radius;
			object.velocity.y *= -1.0f;
			object.velocity *= energyRetain;
		}
		if (object.position.y + object.radius > GetFrameLowerLimit(frame)) {
			object.position.y = GetFrameLowerLimit(frame) - object.radius;
			object.velocity.y *= -1.0f;
			object.velocity *= energyRetain;
		}
	}
}

int main() {
	Window window = { { 1600, 900 }, "Trajectories" };
	Image frame = { window.getSize() };
	Timer timer = {};

	Float2 gravity = { 0, 98.1f };
	Vector<Object> objects = {};

	window.onResize = [&](UInt2 newSize) {
		frame.setSize(newSize);
	};

	bool objectBeingAdded = false;
	Float2 objectAddPosition = {};
	Float2 objectSecondPosition = {};
	window.mouse.left.onPress = [&]() {
		Int2 position = window.mouse.getPosition();
		if (position.x >= 0 && position.x < int(window.getWidth()) && position.y > 0 && position.y < GetFrameLowerLimit(frame)) {
			objectAddPosition = position;
			objectBeingAdded = true;
		}
	};
	window.mouse.left.onDown = [&]() {
		objectSecondPosition = window.mouse.getPosition();
	};
	window.mouse.left.onRelease = [&]() {
		if (objectBeingAdded) {
			Object newObject = {};
			newObject.position = objectSecondPosition;
			newObject.velocity = objectAddPosition - objectSecondPosition;
			objects.push_back(newObject);
			objectBeingAdded = false;
		}
	};

	window.keyboard.r.onPress = [&]() {
		objects.clear();
	};
	window.keyboard.g.onPress = [&]() {
		for (int i = 0; i < 20; i++) {
			Object randomObject = {};
			randomObject.position.x = Random::FLOAT((float)frame.getWidth());
			randomObject.position.y = Random::FLOAT((float)GetFrameLowerLimit(frame));
			randomObject.velocity = Random::VECTOR2<float>(-500.0f, 500.0f);
			objects.push_back(randomObject);
		}
	};

	while (window.process(false)) {
		timer.updateInterval();
		ProcessObjects(objects, timer, frame, gravity);
		DrawBackground(frame);
		DrawObjects(frame, objects);
		DrawInterface(frame, objectBeingAdded, objectAddPosition, objectSecondPosition);
		window.drawImage(frame);
	}
}
