#pragma once
#define _CRT_SECURE_NO_WARNINGS


// Including cpp headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <iterator>
#include <map>

// Including external headers
#include <conio.h>
#include <ws2tcpip.h>
#include <wininet.h>
#include <windows.h>
#include <windowsx.h>
#include <shlwapi.h>
#include <gdiplus.h>
#include <OpenGL/glad.h>
#include <gl/GLU.h>

// Including kl headers
#include "KrimzLib/type.h"
#include "KrimzLib/literal.h"
#include "KrimzLib/constant.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/math.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"
#include "KrimzLib/string.h"
#include "KrimzLib/console.h"
#include "KrimzLib/array.h"
#include "KrimzLib/binary.h"
#include "KrimzLib/file.h"
#include "KrimzLib/thread.h"
#include "KrimzLib/image.h"
#include "KrimzLib/web.h"
#include "KrimzLib/tcp.h"
#include "KrimzLib/opengl.h"
#include "KrimzLib/window.h"
#include "KrimzLib/engine.h"

// Linking .libs
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

// Library intializer and unintializer
namespace _kl_dont_use_ {
	class _kl_initialization_ : private kl::random, private kl::time, private kl::console, private kl::image, private kl::tcp {
	public:
		_kl_initialization_() {
			// random.h
			NewSeed();

			// time.h
			LoadPCFrequency();

			// console.h
			EnableRGB();

			// image.h
			InitGdiPlus();

			// tcp.h
			InitWinSock();
		}
		~_kl_initialization_() {
			// image.h
			UninitGdiPlus();

			// tcp.h
			UninitWinSock();
		}
	};
	_kl_initialization_ _lib_init_;
}
