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
#include <d3d11.h>
#include <d3dcompiler.h>

/* Fixing external defines */
#undef min
#undef max

/* Including kl headers */
// Including kl types
#include "KrimzLib/type.h"
#include "KrimzLib/time.h"
#include "KrimzLib/graphics/color.h"
#include "KrimzLib/vecmath/ivec2.h"
#include "KrimzLib/vecmath/vec2.h"
#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/vecmath/vec4.h"
#include "KrimzLib/vecmath/mat4.h"
#include "KrimzLib/vecmath/vertex.h"
#include "KrimzLib/vecmath/triangle.h"

// Including utility headers
#include "KrimzLib/constant.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/math.h"
#include "KrimzLib/random.h"
#include "KrimzLib/console.h"
#include "KrimzLib/file.h"
#include "KrimzLib/web.h"

// Including special
#include "KrimzLib/graphics/image.h"

// Including opengl types
#include "KrimzLib/gl/glad.h"
#include "KrimzLib/gl/mesh.h"
#include "KrimzLib/gl/texture.h"
#include "KrimzLib/gl/uniform.h"
#include "KrimzLib/gl/shaders.h"
#include "KrimzLib/gl/gl.h"

// Including the window types
#include "KrimzLib/window/keys.h"
#include "KrimzLib/window/mouse.h"
#include "KrimzLib/window/window.h"

// Including directx types
#include "KrimzLib/dx/shaders.h"
#include "KrimzLib/dx/buffer.h"
#include "KrimzLib/dx/gpu.h"

// Including engine types
#include "KrimzLib/gl/renderer/camera.h"
#include "KrimzLib/gl/renderer/renderable/geometry.h"
#include "KrimzLib/gl/renderer/renderable/physics.h"
#include "KrimzLib/gl/renderer/renderable.h"
#include "KrimzLib/gl/renderer/light/ambient.h"
#include "KrimzLib/gl/renderer/light/direct.h"
#include "KrimzLib/gl/renderer/skybox.h"
#include "KrimzLib/gl/renderer.h"

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
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

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
