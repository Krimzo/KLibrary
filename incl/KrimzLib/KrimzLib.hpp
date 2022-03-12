#pragma once

// Math
#include "KrimzLib/math/math.h"
#include "KrimzLib/math/int2.h"
#include "KrimzLib/math/float2.h"
#include "KrimzLib/math/float3.h"
#include "KrimzLib/math/float4.h"
#include "KrimzLib/math/mat3.h"
#include "KrimzLib/math/mat4.h"

// Color
#include "KrimzLib/color/color.h"
#include "KrimzLib/color/image.h"

// Utility
#include "KrimzLib/utility/time.h"
#include "KrimzLib/utility/convert.h"
#include "KrimzLib/utility/random.h"
#include "KrimzLib/utility/console.h"
#include "KrimzLib/utility/file.h"
#include "KrimzLib/utility/web.h"
#include "KrimzLib/utility/thread.h"

// Memory
#include "KrimzLib/memory/pbuffer.h"

// Geometry
#include "KrimzLib/geometry/ray.h"
#include "KrimzLib/geometry/plane.h"
#include "KrimzLib/geometry/sphere.h"
#include "KrimzLib/geometry/vertex.h"
#include "KrimzLib/geometry/triangle.h"

// Window
#include "KrimzLib/window/keys.h"
#include "KrimzLib/window/mouse.h"
#include "KrimzLib/window/window.h"

// GPU
#include "KrimzLib/gpu/gpu.h"

// Render
#include "KrimzLib/render/camera.h"
#include "KrimzLib/render/skybox.h"

// Light
#include "KrimzLib/light/ambient.h"
#include "KrimzLib/light/direct.h"

// Entity
#include "KrimzLib/entity/entity.h"

#ifdef KL_USING_IMGUI
// ImGui
#include "KrimzLib/igui/igui.h"
#endif
