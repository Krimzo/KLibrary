#pragma once
#define _CRT_SECURE_NO_WARNINGS


// Including cpp headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <iterator>
#include <map>
#include <functional>
#include <thread>
#include <algorithm>

// Including c headers
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

// Including external headers
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#include <windowsx.h>
#include "KrimzLib/incl/OpenCL/cl.hpp"
#include "KrimzLib/incl/OpenGL/glad.h"
#include <gl/GLU.h>
#endif

// Including kl headers
#include "KrimzLib/type.h"
#include "KrimzLib/literal.h"
#include "KrimzLib/constant.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/math.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"
#include "KrimzLib/array.h"
#include "KrimzLib/binary.h"
#include "KrimzLib/file.h"
#include "KrimzLib/image.h"
#include "KrimzLib/thread.h"
#include "KrimzLib/console.h"
#include "KrimzLib/string.h"
#include "KrimzLib/web.h"
#ifdef _WIN32
#include "KrimzLib/opencl.h"
#include "KrimzLib/opengl.h"
#include "KrimzLib/window.h"
#include "KrimzLib/engine.h"
#endif

// Linking .libs
#ifdef _WIN32
#pragma comment(lib, "incl/KrimzLib/lib/opencl.lib")	/* !!! Fix the path in case you get linker errors !!! */
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "gdi32.lib")
#endif

namespace kl {
	// Library initializer
	void InitLib() {
		random::SetSeed();
		#ifdef _WIN32
		console::EnableVirtual();
		#endif
	}
}
