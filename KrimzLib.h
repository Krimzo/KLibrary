#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <windows.h>
#include <conio.h>


/* Main namespace */
namespace kl {
	/* Structs */
	typedef unsigned char byte;
	struct color {
		byte r;
		byte g;
		byte b;
	};
	struct size {
		int width;
		int height;
	};

	/* Constants */
	namespace constant {
		const double pi = 3.141592653589f;
		const double toRadians = pi / 180.0f;
		const double toDegrees = 180.0f / pi;
		const color colorBlack = { 0, 0, 0 };
		const color colorWhite = { 200, 200, 200 };
		const color colorGray = { 70, 70, 70 };
		const color colorRed = { 200, 0, 0 };
		const color colorGreen = { 0, 200, 0 };
		const color colorBlue = { 0, 0, 200 };
		const color colorCyan = { 32, 178, 170 };
		const color colorPurple = { 200, 0, 200 };
		const color colorYellow = { 200, 200, 0 };
		const color colorOrange = { 255, 140, 0 };
		const color colorMagenta = { 153, 0, 153 };
		const color colorCrimson = { 102, 0, 0 };
		const color colorSnow = { 255, 255, 255 };
		const color colorSapphire = { 0, 103, 165 };
		const color colorWheat = { 245, 222, 179 };
	}

	/* Miscellaneous stuff */
	namespace misc {
		// Parallel for loop
		void ParallelFor(int startInclusive, int endExclusive, int threadCount, std::function<void(int)> loopBody) {
			// Thread storage
			std::vector<std::thread> cpuThreads(threadCount);
			int countPerThread = (endExclusive - startInclusive) / threadCount;

			// Thread creation
			for (int i = 0; i < threadCount; i++) {
				int loopStart = countPerThread * i + startInclusive;
				int loopEnd = (i == threadCount - 1) ? endExclusive : (loopStart + countPerThread);
				cpuThreads[i] = std::thread([&](int start, int end) {for (int i = start; i < end; i++) { loopBody(i); }}, loopStart, loopEnd);
			}

			// Waiting for the threads to finish
			for (int i = 0; i < threadCount; i++) {
				cpuThreads[i].join();
			}
		}
	}

	/* Console stuff */
	class console {
	public:
		// Sets the console title
		static void SetTitle(std::string text) {
			SetConsoleTitleA(text.c_str());
		}

		// Sets the console cursor position
		static void MoveCursor(short x, short y) {
			SetConsoleCursorPosition(stdConsoleHandle, { x, y });
		}
		static void MoveCursor(COORD position) {
			SetConsoleCursorPosition(stdConsoleHandle, position);
		}

		// Returns the current console size
		static size GetSize() {
			CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
			GetConsoleScreenBufferInfo(stdConsoleHandle, &consoleScreenBufferInfo);
			return {
				consoleScreenBufferInfo.srWindow.Right - consoleScreenBufferInfo.srWindow.Left + 1,
				consoleScreenBufferInfo.srWindow.Bottom - consoleScreenBufferInfo.srWindow.Top + 1
			};
		}
		
		// Enables RGB support for the console
		static void EnableRGB() {
			if (!rgbEnabled) {
				// Get current console mode
				DWORD consoleMode;
				GetConsoleMode(stdConsoleHandle, &consoleMode);
				lastConsoleMode = consoleMode;
				
				// Enable the RGB
				consoleMode = consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				SetConsoleMode(stdConsoleHandle, consoleMode);
				rgbEnabled = true;
			}
		}
		// Disables RGB support for the console
		static void DisableRGB() {
			if (rgbEnabled) {
				SetConsoleMode(stdConsoleHandle, lastConsoleMode);
				rgbEnabled = false;
			}
		}

		// Prints RGB text
		static void Print(std::string text, color textColor = constant::colorWhite) {
			printf("\x1b[38;2;%d;%d;%dm%s", textColor.r, textColor.g, textColor.b, text.c_str());
		}

		// Returns a pressed key
		static char GetInput() {
			char input = 0;
			while (_kbhit()) {
				input = _getch();
			}
			return input;
		}

	private:
		static HANDLE stdConsoleHandle;
		static bool rgbEnabled;
		static DWORD lastConsoleMode;
	};
	HANDLE console::stdConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	bool console::rgbEnabled = false;
	DWORD console::lastConsoleMode = 0;

	class time {
	public:
		// Initialise needed time functions
		static void Init() {
			QueryPerformanceFrequency(&counterLast);
			PCFrequency = double(counterLast.QuadPart);
		}

		// Returns a time since the the last GetElapsed call
		static double GetElapsed() {
			LARGE_INTEGER counterNow;
			QueryPerformanceCounter(&counterNow);
			double time = (counterNow.QuadPart - counterLast.QuadPart) / PCFrequency;
			counterLast = counterNow;
			return time;
		}

	private:
		static LARGE_INTEGER counterLast;
		static double PCFrequency;
	};
	LARGE_INTEGER time::counterLast = {};
	double time::PCFrequency = 0;

	/* My game engine stuff */
	class engine {
	public:
		// Public variables
		double deltaTime = 0;

		// Engine constructor and destructor
		engine() {

		}
		~engine() {
			
		}
		void Delete() {
			this->~engine();
		}

		// Starts the engine
		void Start() {
			engineOn = true;
			EngineLoop();
		}

		// Stops the engine
		void Stop() {
			engineOn = false;
		}

	private:
		// Private variables
		bool engineOn = false;

		// Engine game loop
		void EngineLoop() {
			// Needed for time calculations
			while (engineOn) {
				/* Game input */
				//EngineInput();

				/* Game logic */
				//EngineUpdate();

				/* Applying physics */
				//ObjectPhysics();

				/* Rendering */
				//ObjectRender();
				//StretchDIBits(engineHDC, 0, 0, frameWidth, frameHeight, 0, 0, frameWidth, frameHeight, &frameBuffer[0], &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
				Sleep(14);

				/* Calculating frame time */
				deltaTime = time::GetElapsed();

				/* Updating the title */
				console::SetTitle(std::to_string((int)(1 / deltaTime)));
			}
		}
	};

	/* Class initialiser */
	void Init() {
		console::EnableRGB();
		time::Init();
	}
}
