#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <conio.h>
#include <windows.h>
#include <gdiplus.h>


/* Main namespace */
namespace kl {
	/* Structs and typedefs */
	// Colors/Bitmaps
	typedef unsigned char byte;
	struct color {
		byte r;
		byte g;
		byte b;
	};
	struct bitmap {
		int width = 0;
		int height = 0;
		std::vector<color> pixels;
	};

	// Size/Vectors
	struct size {
		int width;
		int height;
	};
	struct vec2 {
		double x;
		double y;
	};
	struct vec3 {
		double x;
		double y;
		double z;
	};


	/* Constants */
	namespace constant {
		// Doubles
		const double pi = 3.141592653589f;
		const double toRadians = pi / 180.0f;
		const double toDegrees = 180.0f / pi;

		// Colors
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
	class misc {
	public:
		// Parallel for loop
		static void ParallelFor(int startInclusive, int endExclusive, int threadCount, std::function<void(int)> loopBody) {
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

	private:
	};


	/* Math stuff */
	class math {
	public:
		/* Vector math */
		// Returns a given vector lenght
		static double VectorLenght(vec3 vec) {
			return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		}
		static double VectorLenght(vec2 vec) {
			return sqrt(vec.x * vec.x + vec.y * vec.y);
		}

		// Returns a normalized vector
		static vec3 VectorNormalize(vec3 vec) {
			double vecLen = VectorLenght(vec);
			return { vec.x / vecLen, vec.y / vecLen, vec.z / vecLen };
		}
		static vec2 VectorNormalize(vec2 vec) {
			double vecLen = VectorLenght(vec);
			return { vec.x / vecLen, vec.y / vecLen };
		}

		// Returns a dot product of 2 given vectors
		static double VectorDotProd(vec3 a, vec3 b) {
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}
		// Returns a cross product of 2 given vectors
		static vec3 VectorCrossProd(vec3 a, vec3 b) {
			return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
		}

	private:
	};


	/* Console stuff */
	class console {
	public:
		// Sets the console title
		static void SetTitle(std::string text) {
			SetConsoleTitleA(text.c_str());
		}

		// Hides the console cursor
		static void HideCursor() {
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(stdConsoleHandle, &cursorInfo);
			cursorInfo.bVisible = FALSE;
			SetConsoleCursorInfo(stdConsoleHandle, &cursorInfo);
		}

		// Shows the console cursor
		static void ShowCursor() {
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(stdConsoleHandle, &cursorInfo);
			cursorInfo.bVisible = TRUE;
			SetConsoleCursorInfo(stdConsoleHandle, &cursorInfo);
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

		// Changes the console size
		static void SetSize(int width, int height) {
			SMALL_RECT consoleRect = { 0, 0, (short)width - 1, (short)height - 1 };
			SetConsoleScreenBufferSize(stdConsoleHandle, { (short)width, (short)height });
			SetConsoleWindowInfo(stdConsoleHandle, TRUE, &consoleRect);
		}

		// Changes the console font size
		static void SetFont(int width, int height, std::wstring fontName) {
			CONSOLE_FONT_INFOEX cfi;
			cfi.cbSize = sizeof(cfi);
			cfi.nFont = 0;
			cfi.dwFontSize.X = width;
			cfi.dwFontSize.Y = height;
			cfi.FontFamily = FF_DONTCARE;
			cfi.FontWeight = FW_NORMAL;
			wcscpy_s(cfi.FaceName, fontName.c_str());
			SetCurrentConsoleFontEx(stdConsoleHandle, FALSE, &cfi);
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
		static void Print(std::string text, byte r = 255, byte g = 255, byte b = 255) {
			printf("\x1b[38;2;%d;%d;%dm%s", r, g, b, text.c_str());
		}
		static void Print(std::string text, color textColor = constant::colorWhite) {
			printf("\x1b[38;2;%d;%d;%dm%s", textColor.r, textColor.g, textColor.b, text.c_str());
		}

		// Prints RGB block
		static void PrintBlock(byte r, byte g, byte b) {
			printf("\x1b[48;2;%d;%d;%dm ", r, g, b);
		}
		static void PrintBlock(color blockColor) {
			printf("\x1b[48;2;%d;%d;%dm ", blockColor.r, blockColor.g, blockColor.b);
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


	/* Time stuff */
	class time {
	public:
		// Loads the current pc frequency
		static void LoadPCFrequency() {
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


	/* File stuff */
	class file {
	public:
		// Returns a pixel array from the given image
		// You have to link "Gdiplus.lib" if you want to use this function
		static bitmap GetPixels(std::wstring imagePath) {
			// Loads image file
			ULONG_PTR gdiplusToken;
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
			Gdiplus::Bitmap* loadedBitmap = Gdiplus::Bitmap::FromFile(imagePath.c_str());

			// Checks load status
			int lastBitmapStatus = loadedBitmap->GetLastStatus();
			if (lastBitmapStatus) {
				std::wcout << "Couldn't load image \"" << imagePath << "\", status: " << lastBitmapStatus << std::endl;
				char iHateWarnings = getchar();
				exit(-1);
			}

			// Saves data
			bitmap image;
			image.width = loadedBitmap->GetWidth();
			image.height = loadedBitmap->GetHeight();
			for (int y = 0; y < image.height; y++) {
				for (int x = 0; x < image.width; x++) {
					Gdiplus::Color tempPixel;
					loadedBitmap->GetPixel(x, y, &tempPixel);
					image.pixels.push_back({ tempPixel.GetR(), tempPixel.GetG(), tempPixel.GetB() });
				}
			}

			// Clears memory
			delete loadedBitmap;
			Gdiplus::GdiplusShutdown(gdiplusToken);

			// Returns pixel array
			return image;
		}

	private:

	};


	/* My game engine stuff */
	class engine {
	public:
		// Outside functions that user defines
		std::function<void(void)> EngineStart = []() {};
		std::function<void(char)> EngineInput = [](char input) {};
		std::function<void(void)> EngineUpdate = []() {};
		// Engine properties
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
		// Buffers
		std::string frameBuffer = "";
		std::string backgroundBuffer = "";
		std::vector<double> depthBuffer;
		std::vector<double> shadowBuffer;
		// Private variables
		bool engineOn = false;
		int frameWidth = 0;
		int frameHeight = 0;
		size_t pixelCount = 0;

		// Creates a background template
		void BuildBackground() {
			const char* pixelTemplate = "\x1b[48;2;005;005;005m ";
			for (size_t i = 0; i < pixelCount; i++) {
				memcpy(&backgroundBuffer[i * 20], pixelTemplate, 20);
			}
		}

		// Update buffers sizes if the consol size changes
		void CheckConsoleSize() {
			// Getting current console window size
			size consoleSize = console::GetSize();
			if (consoleSize.width != frameWidth || consoleSize.height != frameHeight) {
				// Updating engine properties
				frameWidth = consoleSize.width;
				frameHeight = consoleSize.height;

				// Resizing engine buffers
				pixelCount = size_t(frameWidth) * size_t(frameHeight);
				frameBuffer.resize(pixelCount * 20);
				backgroundBuffer.resize(pixelCount * 20);
				depthBuffer.resize(pixelCount);
				shadowBuffer.resize(pixelCount);
				
				// Building new background buffer
				BuildBackground();

				// Hiding the console cursor
				kl::console::HideCursor();
			}
		}

		// Computing object physics 
		void ObjectPhysics() {

		}

		// Clears all engine buffers
		void ClearBuffers() {
			memcpy(&frameBuffer[0], &backgroundBuffer[0], pixelCount * 20);
			memset(&depthBuffer[0], 0, pixelCount * sizeof(double));
			memset(&shadowBuffer[0], 0, pixelCount * sizeof(double));
		}

		// Rendering objects to the screen
		void ObjectRender() {

		}

		// Displays the frame to the screen
		void RenderFrame() {
			console::MoveCursor(0, 0);
			std::cout << frameBuffer;
		}

		// Engine game loop
		void EngineLoop() {
			// Console setup
			console::HideCursor();
			console::SetFont(4, 4, L"Lucida Console");
			console::SetSize(200, 200);

			// One call before engine start
			EngineStart();
			
			// Needed for time calculations
			while (engineOn) {
				/* Update console size */
				CheckConsoleSize();

				/* Game input */
				EngineInput(console::GetInput());

				/* Game logic */
				EngineUpdate();

				/* Applying physics */
				ObjectPhysics();

				/* Rendering */
				ClearBuffers();
				ObjectRender();
				RenderFrame();

				/* Calculating frame time */
				deltaTime = time::GetElapsed();

				/* Updating the title */
				console::SetTitle(std::to_string((int)(1 / deltaTime)));
			}

			// Console reset
			console::ShowCursor();
			console::SetFont(8, 16, L"Consolas");
			console::SetSize(120, 30);
			console::SetTitle(":)");
		}
	};


	/* Library initialiser */
	void Init() {
		console::EnableRGB();
		time::LoadPCFrequency();
	}
}
