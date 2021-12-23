#pragma once
#define _CRT_SECURE_NO_WARNINGS


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
#include <GL/glad.h>

/* Fixing external macros */
#undef min
#undef max

/* Including kl headers */
// Including misc types
#include "KrimzLib/type.h"
#include "KrimzLib/types/point.h"
#include "KrimzLib/types/size.h"
#include "KrimzLib/types/color.h"
#include "KrimzLib/types/array.h"
#include "KrimzLib/types/stack.h"
#include "KrimzLib/types/time.h"

// Including vector math headers
#include "KrimzLib/types/vecmath/vec2.h"
#include "KrimzLib/types/vecmath/vec3.h"
#include "KrimzLib/types/vecmath/vec4.h"
#include "KrimzLib/types/vecmath/mat4.h"
#include "KrimzLib/types/vecmath/vertex.h"
#include "KrimzLib/types/vecmath/triangle.h"

// Including misc headers
#include "KrimzLib/constant.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/math.h"
#include "KrimzLib/random.h"
#include "KrimzLib/console.h"
#include "KrimzLib/binary.h"
#include "KrimzLib/file.h"
#include "KrimzLib/thread.h"
#include "KrimzLib/web.h"

// Including opengl types
#include "KrimzLib/types/image.h"
#include "KrimzLib/types/opengl/mesh.h"
#include "KrimzLib/types/opengl/texture.h"
#include "KrimzLib/types/opengl/uniform.h"
#include "KrimzLib/types/opengl/shaders.h"
#include "KrimzLib/opengl.h"

// Including the window types
#include "KrimzLib/types/window/keys.h"
#include "KrimzLib/types/window/mouse.h"
#include "KrimzLib/types/window.h"

// Including engine types
#include "KrimzLib/types/renderer/camera.h"
#include "KrimzLib/types/renderer/renderable/geometry.h"
#include "KrimzLib/types/renderer/renderable/physics.h"
#include "KrimzLib/types/renderer/renderable.h"
#include "KrimzLib/types/renderer/light/ambient.h"
#include "KrimzLib/types/renderer/light/direct.h"
#include "KrimzLib/types/renderer/skybox.h"
#include "KrimzLib/types/renderer.h"

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
#pragma comment(lib, "opengl32.lib")

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
