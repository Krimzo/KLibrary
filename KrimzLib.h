#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <functional>
#include <conio.h>
#include <windows.h>
#include <gdiplus.h>	// You need to link "gdiplus.lib"
#include <gl/GL.h>		// You need to link "opengl32.lib"
#include <gl/GLU.h>		// You need to link "opengl32.lib"


/* Main namespace */
namespace kl {
	/* Typedefs and structs */
	// Vectors/Size
	typedef POINT point;
	struct vec2 {
		double x = 0;
		double y = 0;

		// Prints the vector to the console
		void Print() {
			std::cout << "x: " << x << " y: " << y;
		}

		// Returns the vectors lenght
		double Lenght() {
			return sqrt(x * x + y * y);
		}

		// Normalizes a vector
		void Normalize() {
			double vecLen = this->Lenght();
			x /= vecLen;
			y /= vecLen;
		}

		// Retruns a normalized vector
		vec2 Normalized() {
			double vecLen = this->Lenght();
			return { x / vecLen, y / vecLen };
		}
	};
	struct vec3 {
		double x = 0;
		double y = 0;
		double z = 0;

		// Prints the vector to the console
		void Print() {
			std::cout << "x: " << x << " y: " << y << " z: " << z;
		}

		// Returns the vectors lenght
		double Lenght() {
			return sqrt(x * x + y * y + z * z);
		}

		// Normalizes a vector
		void Normalize() {
			double vecLen = this->Lenght();
			x /= vecLen;
			y /= vecLen;
			z /= vecLen;
		}

		// Retruns a normalized vector
		vec3 Normalized() {
			double vecLen = this->Lenght();
			return { x / vecLen, y / vecLen, z / vecLen };
		}
	};
	struct size {
		int width = 0;
		int height = 0;

		// Returns the area
		int Area() {
			return width * height;
		}
	};
	// Colors/Bitmaps
	typedef unsigned char byte;
	struct color {
		byte r = 0;
		byte g = 0;
		byte b = 0;
		byte a = 255;
	};
	struct bitmap {
		int width;
		int height;
		std::vector<color> pixels;

		// Constructor
		bitmap(int width, int height) {
			this->width = width;
			this->height = height;
			pixels.resize(size_t(width) * size_t(height));
		}		

		// Fils the bitmap with solid color
		void FillSolid(color color) {
			std::fill(pixels.begin(), pixels.end(), color);
		}

		// Sets the pixel color
		void SetPixel(int x, int y, color color) {
			if (x >= 0 && x < width && y >= 0 && y < height) {
				pixels[y * size_t(width) + x] = color;
			}
		}
	};
	// Files
	struct filedata {
		std::wstring name;
		std::vector<byte> bytes;
	};


	/* Constants */
	namespace constant {
		// Ints
		const int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		const int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

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
		// Multithreaded for loop
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
	};


	/* Random stuff */
	class random {
	public:
		// Sets the seed for random number generation
		static void SetSeed() {
			srand((unsigned)std::time(NULL));
		}
		
		// Returns a random integer
		static int GetInt(int startInclusive, int endExclusive) {
			return rand() % (endExclusive - startInclusive) + startInclusive;
		}
		
		// Fills a vector with random integers
		static void FillVector(std::vector<int>& vectorToFill, int startInclusive, int endExclusive) {
			for (int i = 0; i < vectorToFill.size(); i++) {
				vectorToFill[i] = GetInt(startInclusive, endExclusive);
			}
		}
	};


	/* Math stuff */
	class math {
	public:
		// Returns a dot product of 2 given vectors
		static double VectorDotProd(vec3 a, vec3 b) {
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}
		// Returns a cross product of 2 given vectors
		static vec3 VectorCrossProd(vec3 a, vec3 b) {
			return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
		}
	};


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
			QueryPerformanceCounter(&counterNow);
			double time = (counterNow.QuadPart - counterLast.QuadPart) / PCFrequency;
			counterLast = counterNow;
			return time;
		}

	private:
		static LARGE_INTEGER counterNow;
		static LARGE_INTEGER counterLast;
		static double PCFrequency;
	};
	LARGE_INTEGER time::counterNow = {};
	LARGE_INTEGER time::counterLast = {};
	double time::PCFrequency = 0;


	/* File stuff */
	class file {
	public:
		// Returns a string from a given text file
		static std::string GetText(std::wstring fileName) {
			std::stringstream textBuffer;
			std::ifstream fileStream(fileName);
			if (fileStream.is_open()) {
				textBuffer << fileStream.rdbuf();
			}
			else {
				std::wcout << "Couldn't load text file \"" << fileName << "\"" << std::endl;
				char iHateWarnings = getchar();
				exit(69);
			}
			fileStream.close();
			return textBuffer.str();
		}

		// Returns a filedata from a given file
		static filedata GetBytes(std::wstring fileName) {
			filedata tempFileData = { fileName };
			std::ifstream fileStream(fileName, std::ios::binary);
			if (!fileStream.is_open()) {
				std::wcout << "Couldn't load file \"" << fileName << "\"" << std::endl;
				char iHateWarnings = getchar();
				exit(69);
			}
			fileStream.seekg(0, std::ios::end);
			size_t fileLen = fileStream.tellg();
			fileStream.seekg(0, std::ios::beg);
			tempFileData.bytes.resize(fileLen);
			fileStream.read((char*)&tempFileData.bytes[0], fileLen);
			fileStream.close();
			return tempFileData;
		}

		// Returns a bitmap from the given image file
		// You have to link "gdiplus.lib" if you want to use this function
		static bitmap GetPixels(std::wstring imagePath) {
			// Loads image file
			ULONG_PTR gdiplusToken;
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
			Gdiplus::Bitmap* loadedBitmap = Gdiplus::Bitmap::FromFile(imagePath.c_str());

			// Checks load status
			int lastBitmapStatus = loadedBitmap->GetLastStatus();
			if (lastBitmapStatus) {
				std::wcout << "Couldn't load image file \"" << imagePath << "\", status: " << lastBitmapStatus << std::endl;
				char iHateWarnings = getchar();
				exit(69);
			}

			// Saves data
			bitmap tempBitmap(loadedBitmap->GetWidth(), loadedBitmap->GetHeight());
			for (int y = 0; y < tempBitmap.height; y++) {
				for (int x = 0; x < tempBitmap.width; x++) {
					Gdiplus::Color tempPixel;
					loadedBitmap->GetPixel(x, y, &tempPixel);
					tempBitmap.pixels[y * size_t(tempBitmap.width) + x] = { tempPixel.GetR(), tempPixel.GetG(), tempPixel.GetB() };
				}
			}

			// Clears memory
			delete loadedBitmap;
			Gdiplus::GdiplusShutdown(gdiplusToken);

			// Return created bitmap
			return tempBitmap;
		}
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
		static void SetCursorPos(short x, short y) {
			SetConsoleCursorPosition(stdConsoleHandle, { x, y });
		}
		static void SetCursorPos(COORD position) {
			SetConsoleCursorPosition(stdConsoleHandle, position);
		}

		// Returns the current console size
		static size GetSize() {
			CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
			GetConsoleScreenBufferInfo(stdConsoleHandle, &consoleScreenBufferInfo);
			return {
				int(consoleScreenBufferInfo.srWindow.Right) - int(consoleScreenBufferInfo.srWindow.Left) + 1,
				int(consoleScreenBufferInfo.srWindow.Bottom) - int(consoleScreenBufferInfo.srWindow.Top) + 1
			};
		}

		// Changes the console buffer size
		static void SetBufferSize(int width, int height) {
			SetConsoleScreenBufferSize(stdConsoleHandle, { (short)width, (short)height });
		}

		// Changes the console size
		static void SetSize(int width, int height) {
			SetBufferSize(width, height);
			SMALL_RECT consoleRect = { 0, 0, (short)width - 1, (short)height - 1 };	
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
		static void Print(std::string data, color textColor = constant::colorWhite) {
			printf("\x1b[38;2;%d;%d;%dm%s", textColor.r, textColor.g, textColor.b, data.c_str());
		}
		static void Print(int data, color textColor = constant::colorWhite) {
			printf("\x1b[38;2;%d;%d;%dm%d", textColor.r, textColor.g, textColor.b, data);
		}
		static void Print(double data, color textColor = constant::colorWhite) {
			printf("\x1b[38;2;%d;%d;%dm%lf", textColor.r, textColor.g, textColor.b, data);
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

		// Waits until the wanted key is pressed
		static void WaitFor(char toWaitFor, bool printMessage = false) {
			if (printMessage) {
				std::cout << "Waiting for '" << toWaitFor << "'";
			}
			while (_getch() != toWaitFor);
		}

	private:
		static HANDLE stdConsoleHandle;
		static bool rgbEnabled;
		static DWORD lastConsoleMode;
	};
	HANDLE console::stdConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	bool console::rgbEnabled = false;
	DWORD console::lastConsoleMode = 0;


	/* WIN32 stuff */
	class window {
	public:
		// Public window properties
		LPCWSTR name = NULL;
		HWND hwnd = NULL;
		HDC hdc = NULL;
		HGLRC hglrc = NULL;
		WPARAM keyDown = 0;
		bool lmbDown = false;
		bool rmbDown = false;
		point mousePos = {};

		// Window constructor and destructor
		// You need to link "opengl32.lib" if you want to use OpenGL
		window(int windowWidth, int windowHeight, const wchar_t* windowName, bool resizeable = true, bool useOpenGL = false) {
			// Start a new window thread
			bool windowCreated = false;
			std::thread windowThread([&]() {
				// Define windowapi window class
				name = windowName;
				WNDCLASS windowClass = {};
				windowClass.lpfnWndProc = DefWindowProcW;
				windowClass.hInstance = hInstance;
				windowClass.lpszClassName = name;
				windowClass.style = CS_OWNDC;
				RegisterClassW(&windowClass);

				// Create window
				DWORD windowStyle = resizeable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
				RECT adjustedWindowSize = { 0, 0, windowWidth, windowHeight };
				AdjustWindowRectEx(&adjustedWindowSize, windowStyle, NULL, NULL);
				windowWidth = (adjustedWindowSize.right - adjustedWindowSize.left);
				windowHeight = (adjustedWindowSize.bottom - adjustedWindowSize.top);
				hwnd = CreateWindowExW(NULL, name, name, windowStyle, (constant::ScreenWidth / 2 - windowWidth / 2), (constant::ScreenHeight / 2 - windowHeight / 2), windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
				if (!hwnd) { exit(69); }
				ShowWindow(hwnd, SW_SHOW);
				hdc = GetDC(hwnd);
				windowCreated = true;

				// Bitmapinfo setup
				bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
				bitmapInfo.bmiHeader.biPlanes = 1;
				bitmapInfo.bmiHeader.biBitCount = 32;
				bitmapInfo.bmiHeader.biCompression = BI_RGB;

				// OpenGL setup
				if (useOpenGL) {
					PIXELFORMATDESCRIPTOR pfd = {
					sizeof(PIXELFORMATDESCRIPTOR),
					1,
					PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
					PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
					32,                   // Colordepth of the framebuffer.
					0, 0, 0, 0, 0, 0,
					0,
					0,
					0,
					0, 0, 0, 0,
					24,                   // Number of bits for the depthbuffer
					8,                    // Number of bits for the stencilbuffer
					0,                    // Number of Aux buffers in the framebuffer.
					PFD_MAIN_PLANE,
					0,
					0, 0, 0
					};
					int pixelFormat = ChoosePixelFormat(hdc, &pfd);
					if (!pixelFormat) { exit(69); }
					SetPixelFormat(hdc, pixelFormat, &pfd);
					hglrc = wglCreateContext(hdc);
					wglMakeCurrent(hdc, hglrc);
					RECT clientArea = {};
					GetClientRect(hwnd, &clientArea);
					glViewport(clientArea.left, clientArea.top, clientArea.right, clientArea.bottom);
				}

				// Window message loop
				point tempMouseCoords = {};
				while (GetMessageW(&windowMessage, hwnd, 0, 0) > 0) {
					// Handling window messages
					DispatchMessageW(&windowMessage);
					switch (windowMessage.message) {
					case WM_KEYDOWN:
						keyDown = windowMessage.wParam;
						break;

					case WM_KEYUP:
						keyDown = 0;
						break;

					case WM_LBUTTONDOWN:
						lmbDown = true;
						break;

					case WM_LBUTTONUP:
						lmbDown = false;
						break;

					case WM_RBUTTONDOWN:
						rmbDown = true;
						break;

					case WM_RBUTTONUP:
						rmbDown = false;
						break;

					case WM_MOUSEMOVE:
						GetCursorPos(&tempMouseCoords);
						ScreenToClient(hwnd, &tempMouseCoords);
						mousePos = tempMouseCoords;
						break;
					}
				}

				// Memory release
				if (useOpenGL) {
					wglMakeCurrent(hdc, NULL);
					wglDeleteContext(hglrc);
				}
				ReleaseDC(hwnd, hdc);
				DestroyWindow(hwnd);
				UnregisterClassW(name, hInstance);
			});
			windowThread.detach();
			while (!windowCreated);
		}
		~window() {
			SendMessageW(hwnd, WM_CLOSE, 0, 0);
		}

		// Sets the window title
		void SetTitle(std::string data) {
			SetWindowTextA(hwnd, data.c_str());
		}
		void SetTitle(int data) {
			SetWindowTextA(hwnd, std::to_string(data).c_str());
		}
		void SetTitle(double data) {
			SetWindowTextA(hwnd, std::to_string(data).c_str());
		}

		// Sets the pixels of the window
		void DisplayBitmap(bitmap& toDraw, point position = { 0, 0 }) {
			bitmapInfo.bmiHeader.biWidth = toDraw.width;
			bitmapInfo.bmiHeader.biHeight = toDraw.height;
			StretchDIBits(hdc, position.x, (toDraw.height - 1) + position.y, toDraw.width, -toDraw.height, 0, 0, toDraw.width, toDraw.height, &toDraw.pixels[0], &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		}

	private:
		// Private window properties
		HINSTANCE hInstance = GetModuleHandleW(NULL);
		MSG windowMessage = {};
		BITMAPINFO bitmapInfo = {};
	};


	/* Game engine stuff */
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
			console::SetCursorPos(0, 0);
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
			console::SetTitle("Engine off");
			system("cls");
		}
	};


	/* Library initialiser */
	void Init() {
		random::SetSeed();
		console::EnableRGB();
		time::LoadPCFrequency();
	}
}
