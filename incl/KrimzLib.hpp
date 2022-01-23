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
#include "KrimzLib/vecmath/mat3.h"
#include "KrimzLib/vecmath/mat4.h"
#include "KrimzLib/vecmath/vertex3D.h"
#include "KrimzLib/vecmath/vertex2D.h"
#include "KrimzLib/vecmath/triangle.h"
#include "KrimzLib/math.h"

// Including utility
#include "KrimzLib/time.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/random.h"
#include "KrimzLib/console.h"
#include "KrimzLib/file.h"
#include "KrimzLib/web.h"

// Including special
#include "KrimzLib/graphics/image.h"

// Including gl
#include "KrimzLib/gl/glad.h"
#include "KrimzLib/gl/gl.h"
#include "KrimzLib/gl/mesh.h"
#include "KrimzLib/gl/texture.h"
#include "KrimzLib/gl/uniform.h"
#include "KrimzLib/gl/shaders.h"

// Including window
#include "KrimzLib/window/keys.h"
#include "KrimzLib/window/mouse.h"
#include "KrimzLib/window/window.h"

// Including 2D renderer
#include "KrimzLib/renderer2D/object2D.h"
#include "KrimzLib/renderer2D/renderer2D.h"

// Including 3D renderer
#include "KrimzLib/renderer3D/object3D.h"
#include "KrimzLib/renderer3D/camera.h"
#include "KrimzLib/renderer3D/skybox.h"
#include "KrimzLib/renderer3D/light/ambient.h"
#include "KrimzLib/renderer3D/light/direct.h"
#include "KrimzLib/renderer3D/renderer3D.h"

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
