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
#include <windowsx.h>
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
		int width = 0;
		int height = 0;
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
	// Graphics/OpenGL
	struct colorf {
		float r = 0;
		float g = 0;
		float b = 0;
		float a = 1;
	};
	struct vertex {
		float x = 0;
		float y = 0;
		float z = 0;
		float u = 0;
		float v = 0;
		float w = 0;
		colorf color = {};
	};
	struct triangle {
		vertex vertices[3] = {};
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
		const color colorBlack = { 0, 0, 0, 255 };
		const colorf colorfBlack = { 0.00f, 0.00f, 0.00f, 1.00f };
		const color colorWhite = { 200, 200, 200, 255 };
		const colorf colorfWhite = { 0.78f, 0.78f, 0.78f, 1.00f };
		const color colorGray = { 70, 70, 70, 255 };
		const colorf colorfGray = { 0.27f, 0.27f, 0.27f, 1.00f };
		const color colorRed = { 200, 0, 0, 255 };
		const colorf colorfRed = { 0.78f, 0.00f, 0.00f, 1.00f };
		const color colorGreen = { 0, 200, 0, 255 };
		const colorf colorfGreen = { 0.00f, 0.78f, 0.00f, 1.00f };
		const color colorBlue = { 0, 0, 200, 255 };
		const colorf colorfBlue = { 0.00f, 0.00f, 0.78f, 1.00f };
		const color colorCyan = { 32, 178, 170, 255 };
		const colorf colorfCyan = { 0.13f, 0.70f, 0.67f, 1.00f };
		const color colorPurple = { 200, 0, 200, 255 };
		const colorf colorfPurple = { 0.78f, 0.00f, 0.78f, 1.00f };
		const color colorYellow = { 200, 200, 0, 255 };
		const colorf colorfYellow = { 0.78f, 0.78f, 0.00f, 1.00f };
		const color colorOrange = { 255, 140, 0, 255 };
		const colorf colorfOrange = { 1.00f, 0.55f, 0.00f, 1.00f };
		const color colorMagenta = { 153, 0, 153, 255 };
		const colorf colorfMagenta = { 0.60f, 0.00f, 0.60f, 1.00f };
		const color colorCrimson = { 102, 0, 0, 255 };
		const colorf colorfCrimson = { 0.40f, 0.00f, 0.00f, 1.00f };
		const color colorSnow = { 255, 255, 255, 255 };
		const colorf colorfSnow = { 1.00f, 1.00f, 1.00f, 1.00f };
		const color colorSapphire = { 0, 103, 165, 255 };
		const colorf colorfSapphire = { 0.00f, 0.40f, 0.65f, 1.00f };
		const color colorWheat = { 245, 222, 179, 255 };
		const colorf colorfWheat = { 0.96f, 0.87f, 0.70f, 1.00f };
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
					tempBitmap.pixels[y * size_t(tempBitmap.width) + x] = { tempPixel.GetR(), tempPixel.GetG(), tempPixel.GetB(), 255 };
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


	/* OpenGL stuff */
	class opengl {
	public:
		// Set the whole screen to a given color
		static void ClearScreen(colorf color = { 0, 0, 0, 1 }) {
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		// Draws a triangle on the screen
		static void DrawTriangle(triangle tr) {
			glBegin(GL_TRIANGLES);
			for (int i = 0; i < 3; i++) {
				glColor3f(tr.vertices[i].color.r, tr.vertices[i].color.g, tr.vertices[i].color.b);
				glVertex3f(tr.vertices[i].x, tr.vertices[i].y, tr.vertices[i].z);
			}
			glEnd();
		}

		// Flips the front and back hdc buffers
		static void FlipBuffers(HDC hdc) {
			SwapBuffers(hdc);
		}
	};


	/* WIN32 stuff */
	class window {
	public:
		// Winapi properties
		LPCWSTR name = NULL;
		HWND hwnd = NULL;
		HDC hdc = NULL;
		
		// Input properties
		WPARAM keyDown = 0;
		bool lmbDown = false;
		bool rmbDown = false;
		point mousePos = {};

		// OpenGL properties
		HGLRC hglrc = NULL;
		std::function<void(void)> OpenGLStart = []() {};
		std::function<void(void)> OpenGLUpdate = []() {};

		// Window creation and deletion
		// You need to link "opengl32.lib" if you want to use OpenGL
		void New(int windowWidth, int windowHeight, const wchar_t* windowName, bool resizeable = true, bool useOpenGL = false) {
			if (!windowCreated) {
				// Start a new window thread
				std::thread windowThread([&]() {
					// Define windowapi window class
					name = windowName;
					WNDCLASS windowClass = {};
					windowClass.lpfnWndProc = DefWindowProc;
					windowClass.hInstance = hInstance;
					windowClass.lpszClassName = name;
					windowClass.style = CS_OWNDC;
					RegisterClass(&windowClass);

					// Create window
					DWORD windowStyle = resizeable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
					RECT adjustedWindowSize = { 0, 0, windowWidth, windowHeight };
					AdjustWindowRectEx(&adjustedWindowSize, windowStyle, NULL, NULL);
					windowWidth = (adjustedWindowSize.right - adjustedWindowSize.left);
					windowHeight = (adjustedWindowSize.bottom - adjustedWindowSize.top);
					hwnd = CreateWindowEx(NULL, name, name, windowStyle, (constant::ScreenWidth / 2 - windowWidth / 2), (constant::ScreenHeight / 2 - windowHeight / 2), windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
					if (!hwnd) { exit(69); }
					ShowWindow(hwnd, SW_SHOW);
					hdc = GetDC(hwnd);
					windowCreated = true;
					while (!mainThreadOut);

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
					if (useOpenGL) {
						OpenGLStart();
						while (GetMessage(&windowMessage, hwnd, 0, 0) > 0) {
							HandleMessage();
							OpenGLUpdate();
						}
					}
					else {
						while (GetMessage(&windowMessage, hwnd, 0, 0) > 0) {
							HandleMessage();
						}
					}

					// Memory release
					if (useOpenGL) {
						wglMakeCurrent(NULL, NULL);
						ReleaseDC(hwnd, hdc);
						wglDeleteContext(hglrc);
					}
					DestroyWindow(hwnd);
					UnregisterClass(name, hInstance);
					windowDeleted = true;
				});
				windowThread.detach();
				while (!windowCreated);
				mainThreadOut = true;
			}
		}
		void Delete() {
			if (windowCreated) {
				// Wait until window/thread gets destroyed
				PostMessage(hwnd, WM_CLOSE, 0, 0);
				while (!windowDeleted);

				// Resetting properties
				windowCreated = false;
				mainThreadOut = false;
				windowDeleted = false;
			}
		}
		~window() {
			this->Delete();
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
		BITMAPINFO bitmapInfo = {};
		MSG windowMessage = {};
		bool windowCreated = false;
		bool mainThreadOut = false;
		bool windowDeleted = false;

		// Handles the windows message
		void HandleMessage() {
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
				mousePos = { GET_X_LPARAM(windowMessage.lParam),  GET_Y_LPARAM(windowMessage.lParam) };
				break;

			default:
				DispatchMessageW(&windowMessage);
				break;
			}
		}
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
			engineRunning = true;
			EngineLoop();
		}

		// Stops the engine
		void Stop() {
			engineRunning = false;
		}

	private:
		// Buffers
		std::string frameBuffer = "";
		std::string backgroundBuffer = "";
		std::vector<double> depthBuffer;
		std::vector<double> shadowBuffer;
		// Private variables
		bool engineRunning = false;
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
			while (engineRunning) {
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
