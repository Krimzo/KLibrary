#pragma once
#include "KrimzLib/types.h"
#include "KrimzLib/constants.h"
#include "KrimzLib/math.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"
#include "KrimzLib/file.h"		// You need to link gdiplus.lib (just link you already have it)
#include "KrimzLib/thread.h"
#include "KrimzLib/console.h"
#include "KrimzLib/opengl.h"	// You need to link opengl32.lib (just link you already have it)
#include "KrimzLib/opencl.h"	// You need to link OpenCL.lib (from the lib folder)
#include "KrimzLib/window.h"
#include "KrimzLib/engine.h"


namespace kl {
	// Lbrary intialiser
	void InitLib() {
		random::SetSeed();
		console::EnableRGB();
		time::LoadPCFrequency();
		opencl::Init();
	}
}
