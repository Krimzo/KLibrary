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
		kl::id programID;
		kl::id uniformID;

	public:
		uniform();
		uniform(kl::id programID, kl::id uniformID);

		// Setts the data of an uniform
		void setData(int data) const;
		void setData(float data) const;
		void setData(const kl::vec2& data) const;
		void setData(const kl::vec3& data) const;
		void setData(const kl::vec4& data) const;
		void setData(const kl::mat3& data) const;
		void setData(const kl::mat4& data) const;
	};
}
