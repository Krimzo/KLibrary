#pragma once
#include "KrimzLib/types.h"
#include "KrimzLib/constant.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/math.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"
#include "KrimzLib/binary.h"
#include "KrimzLib/file.h"
#include "KrimzLib/thread.h"
#include "KrimzLib/web.h"
#include "KrimzLib/console.h"
#include "KrimzLib/opencl.h"
#include "KrimzLib/opengl.h"
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
