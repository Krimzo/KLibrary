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
#include <ws2tcpip.h>
#include <wininet.h>
#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>
#include "KrimzLib/incl/OpenCL/cl.hpp"
#include "KrimzLib/incl/OpenGL/glad.h"
#include <gl/GLU.h>

// Including kl headers
#include "KrimzLib/type.h"
#include "KrimzLib/literal.h"
#include "KrimzLib/constant.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/math.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"
#include "KrimzLib/console.h"
#include "KrimzLib/binary.h"
#include "KrimzLib/file.h"
#include "KrimzLib/thread.h"
#include "KrimzLib/net.h"
#include "KrimzLib/opencl.h"
#include "KrimzLib/opengl.h"
#include "KrimzLib/window.h"
#include "KrimzLib/engine.h"

// Linking .libs
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "incl/KrimzLib/lib/opencl.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")


namespace kl
{
	// Lbrary intialiser
	void InitLib(bool initOpenCL = false)
	{
		random::SetSeed();
		time::LoadPCFrequency();
		console::EnableRGB();
		net::InitWinSock();
		file::InitGdiplus();
		if (initOpenCL)
			opencl::Init();
	}

	// Lbrary unintialiser
	void UninitLib()
	{
		console::DisableRGB();
		net::UninitWinSock();
		file::UninitGdiplus();
		opencl::Uninit();
	}
}
