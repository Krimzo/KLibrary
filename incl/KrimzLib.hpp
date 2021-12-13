#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define FREEGLUT_STATIC
#define GLEW_STATIC


/* Including cpp headers */
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

/* Including external headers */
#include <conio.h>
#include <windows.h>
#include <windowsx.h>
#include <wininet.h>
#include <gdiplus.h>
#include <GL/glew.h>
#include <GL/glut.h>

/* Fixing external macros */
#undef min
#undef max

/* Including kl headers */
// Including misc types
#include "KrimzLib/type.h"
#include "KrimzLib/types/point.h"
#include "KrimzLib/types/size.h"
#include "KrimzLib/types/color.h"
#include "KrimzLib/types/colorf.h"
#include "KrimzLib/types/array.h"

// Including vector math headers
#include "KrimzLib/types/vecmath/vec2.h"
#include "KrimzLib/types/vecmath/vec3.h"
#include "KrimzLib/types/vecmath/vec4.h"
#include "KrimzLib/types/vecmath/mat4.h"
#include "KrimzLib/types/vecmath/vertex.h"

// Including cuda headers
#ifdef __CUDACC__
#include "KrimzLib/types/cuda/gpubuffer.cuh"
#endif

// Including misc headers
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

// Including opengl types
#include "KrimzLib/types/opengl/vbo.h"
#include "KrimzLib/types/opengl/ibo.h"
#include "KrimzLib/types/opengl/shaders.h"
#include "KrimzLib/types/opengl/texture.h"
#include "KrimzLib/types/image.h"
#include "KrimzLib/opengl.h"

// Including engine types
#include "KrimzLib/types/engine/mouse.h"
#include "KrimzLib/types/engine/camera.h"
#include "KrimzLib/types/engine/gameobject.h"
#include "KrimzLib/types/window.h"
#include "KrimzLib/types/engine.h"

/* Linking libs */
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
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "freeglut_static.lib")

/* Library intializer and unintializer */
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
