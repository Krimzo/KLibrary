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
#include "KrimzLib/renderer/camera.h"

// Including the window types
#include "KrimzLib/renderer/gl/glad.h"
#include "KrimzLib/window/keys.h"
#include "KrimzLib/window/mouse.h"
#include "KrimzLib/window/window.h"

// Including kl opengl
#include "KrimzLib/renderer/gl/mesh.h"
#include "KrimzLib/renderer/gl/texture.h"
#include "KrimzLib/renderer/gl/uniform.h"
#include "KrimzLib/renderer/gl/shaders.h"
#include "KrimzLib/renderer/gl/skybox.h"
#include "KrimzLib/renderer/gl/light/ambient.h"
#include "KrimzLib/renderer/gl/light/direct.h"
#include "KrimzLib/renderer/gl/gl.h"

// Including kl directx
#include "KrimzLib/renderer/dx/mesh.h"
#include "KrimzLib/renderer/dx/texture.h"
#include "KrimzLib/renderer/dx/cbuffer.h"
#include "KrimzLib/renderer/dx/shaders.h"
#include "KrimzLib/renderer/dx/gpu.h"

// Including rendering headers
#include "KrimzLib/renderer/renderable/geometry.h"
#include "KrimzLib/renderer/renderable/physics.h"
#include "KrimzLib/renderer/renderable/renderable.h"
#include "KrimzLib/renderer/gl/renderer.h"
#include "KrimzLib/renderer/dx/renderer.h"

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
