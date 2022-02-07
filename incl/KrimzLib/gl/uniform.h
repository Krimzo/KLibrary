#pragma once

#include <iostream>

#include "KrimzLib/gl/gl.h"
#include "KrimzLib/vecmath/vec2.h"
#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/vecmath/vec4.h"
#include "KrimzLib/vecmath/mat3.h"
#include "KrimzLib/vecmath/mat4.h"


namespace kl {
	class uniform {
	private:
		kl::id programID = NULL;
		kl::id uniformID = NULL;

	public:
		uniform();
		uniform(kl::id programID, kl::id uniformID);

		// Setts the data of an uniform
		void setData(int data);
		void setData(float data);
		void setData(const kl::vec2& data);
		void setData(const kl::vec3& data);
		void setData(const kl::vec4& data);
		void setData(const kl::mat3& data);
		void setData(const kl::mat4& data);
	};
}
