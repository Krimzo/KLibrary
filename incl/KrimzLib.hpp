#pragma once
#define _CRT_SECURE_NO_WARNINGS


// Including cpp headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <thread>

// Including external headers
#include <conio.h>
#include <windows.h>
#include <windowsx.h>
#include <wininet.h>
#include <gdiplus.h>
#include <gl/GL.h>
#include <gl/GLU.h>

// Fixing external macros
#undef min
#undef max

// Including kl headers
#include "KrimzLib/type.h"
#include "KrimzLib/types/point.h"
#include "KrimzLib/types/size.h"
#include "KrimzLib/types/vec2.h"
#include "KrimzLib/types/vec3.h"
#include "KrimzLib/types/color.h"
#include "KrimzLib/types/array.h"
#include "KrimzLib/types/gpubuffer.cuh"

#include "KrimzLib/literal.h"
#include "KrimzLib/constant.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/math.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"
#include "KrimzLib/string.h"
#include "KrimzLib/console.h"
#include "KrimzLib/binary.h"
#include "KrimzLib/file.h"
#include "KrimzLib/thread.h"
#include "KrimzLib/web.h"

#include "KrimzLib/types/image.h"
#include "KrimzLib/types/mouse.h"
#include "KrimzLib/types/camera.h"
#include "KrimzLib/types/vertex.h"
#include "KrimzLib/types/triangle.h"
#include "KrimzLib/types/gameobject.h"

#include "KrimzLib/opengl.h"
#include "KrimzLib/types/window.h"
#include "KrimzLib/types/engine.h"

// Linking libs
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winspool.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

// Library intializer and unintializer
namespace _kl_dont_use_ {
	class _kl_initialization_ : private kl::random, private kl::time, private kl::console, private kl::image {
	public:
		_kl_initialization_() {
			// random.h
			newSeed();

			// time.h
			loadPCFrequency();
			staticGetElapsed();
			staticStopwatchReset();

			// console.h
			enableRGB();

			// image.h
			initGdiPlus();
		}
		~_kl_initialization_() {
			// image.h
			uninitGdiPlus();
		}
	};
	_kl_initialization_ _lib_init_;
}
