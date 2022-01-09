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

/* Fixing external defines */
#undef min
#undef max

/* Including kl headers */
// Including graphics
#include "KrimzLib/graphics/color.h"

// Including math
#include "KrimzLib/vecmath/ivec2.h"
#include "KrimzLib/vecmath/vec2.h"
#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/vecmath/vec4.h"
#include "KrimzLib/vecmath/mat4.h"
#include "KrimzLib/vecmath/vertex.h"
#include "KrimzLib/vecmath/triangle.h"
#include "KrimzLib/math.h"

// Including utility headers
#include "KrimzLib/time.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/random.h"
#include "KrimzLib/console.h"
#include "KrimzLib/file.h"
#include "KrimzLib/web.h"

// Including special
#include "KrimzLib/graphics/image.h"
#include "KrimzLib/renderer/camera.h"
#include "KrimzLib/renderer/gl/glad.h"

// Including the window types
#include "KrimzLib/window/keys.h"
#include "KrimzLib/window/mouse.h"
#include "KrimzLib/window/window.h"

// Including the renderer headers
#include "KrimzLib/renderer/gl/gl.h"
#include "KrimzLib/renderer/mesh.h"
#include "KrimzLib/renderer/texture.h"
#include "KrimzLib/renderer/uniform.h"
#include "KrimzLib/renderer/shaders.h"
#include "KrimzLib/renderer/skybox.h"
#include "KrimzLib/renderer/light/ambient.h"
#include "KrimzLib/renderer/light/direct.h"
#include "KrimzLib/renderer/renderable/geometry.h"
#include "KrimzLib/renderer/renderable/physics.h"
#include "KrimzLib/renderer/renderable/renderable.h"
#include "KrimzLib/renderer/renderer.h"

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
