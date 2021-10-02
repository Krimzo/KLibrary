#pragma once
#include "KrimzLib/types.h"
#include "KrimzLib/constants.h"
#include "KrimzLib/math.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"
#include "KrimzLib/file.h"		// gdiplus.lib	(just link it you already have it on your pc)
#include "KrimzLib/thread.h"
#include "KrimzLib/console.h"
#include "KrimzLib/opengl.h"	// opengl32.lib (just link it you already have it on your pc)
#include "KrimzLib/opencl.h"	// opencl.lib	(link it from the lib folder)
#include "KrimzLib/window.h"
#include "KrimzLib/engine.h"


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
