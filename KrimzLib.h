#pragma once
#include "KrimzLib/KrimzTypes.h"
#include "KrimzLib/KrimzConstants.h"
#include "KrimzLib/KrimzMath.h"
#include "KrimzLib/KrimzRandom.h"
#include "KrimzLib/KrimzTime.h"
#include "KrimzLib/KrimzFile.h"
#include "KrimzLib/KrimzThread.h"
#include "KrimzLib/KrimzConsole.h"
#include "KrimzLib/KrimzOpenGL.h"
#include "KrimzLib/KrimzWindow.h"
#include "KrimzLib/KrimzEngine.h"


namespace kl {
	void Init() {
		random::SetSeed();
		console::EnableRGB();
		time::LoadPCFrequency();
	}
}
