#pragma once
#include "KrimzLib/types.h"
#include "KrimzLib/constants.h"
#include "KrimzLib/math.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"
#include "KrimzLib/file.h"
#include "KrimzLib/thread.h"
#include "KrimzLib/console.h"
#include "KrimzLib/opengl.h"
#include "KrimzLib/opencl.h"
#include "KrimzLib/window.h"
#include "KrimzLib/engine.h"


namespace kl {
	void Init() {
		random::SetSeed();
		console::EnableRGB();
		time::LoadPCFrequency();
	}
}
