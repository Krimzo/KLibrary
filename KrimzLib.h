#pragma once

// Including cpp headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <functional>
#include <thread>

// Including external headers
#include <conio.h>
#include <WinSock2.h>
#include <wininet.h>
#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>
#include "KrimzLib/incl/OpenCL/cl.hpp"
#include "KrimzLib/incl/OpenGL/glad.h"
#include <gl/GLU.h>

// Including kl headers
#include "KrimzLib/types.h"
#include "KrimzLib/constant.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/math.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"
#include "KrimzLib/console.h"
#include "KrimzLib/binary.h"
#include "KrimzLib/file.h"
#include "KrimzLib/thread.h"
#include "KrimzLib/web.h"
#include "KrimzLib/opencl.h"
#include "KrimzLib/opengl.h"
#include "KrimzLib/window.h"
#include "KrimzLib/engine.h"

// Linking libs
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "gdiplus.lib")
#pragma comment(lib, "incl/KrimzLib/lib/opencl.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")


namespace kl
{
	// Lbrary intialiser
	void InitLib()
	{
		random::SetSeed();
		console::EnableRGB();
		time::LoadPCFrequency();
	}

	// Lbrary unintialiser
	void UninitLib()
	{
		console::DisableRGB();
	}
}
