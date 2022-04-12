#include "math/mat4.h"

#include <iostream>

#include "utility/convert.h"


kl::mat4::mat4()
{
	setIdentity();
}

// Getter
float& kl::mat4::operator[](int i)
{
	return data[i];
}

// Addition
kl::mat4 kl::mat4::add(const kl::mat4& obj) const
{
	kl::mat4 temp;
	for (int i = 0; i < 16; i++)
		temp[i] = data[i] + obj.data[i];
	return temp;
}
kl::mat4 kl::mat4::operator+(const kl::mat4& obj) const
{
	return this->add(obj);
}
void kl::mat4::operator+=(const kl::mat4& obj)
{
	for (int i = 0; i < 16; i++)
		data[i] += obj.data[i];
}

// Subtraction
kl::mat4 kl::mat4::sub(const kl::mat4& obj) const
{
	kl::mat4 temp;
	for (int i = 0; i < 16; i++)
		temp[i] = data[i] - obj.data[i];
	return temp;
}
kl::mat4 kl::mat4::operator-(const kl::mat4& obj) const
{
	return this->sub(obj);
}
void kl::mat4::operator-=(const kl::mat4& obj)
{
	for (int i = 0; i < 16; i++)
		data[i] -= obj.data[i];
}

// Multiplication
kl::mat4 kl::mat4::mul(float a) const
{
	kl::mat4 temp;
	for (int i = 0; i < 16; i++)
		temp[i] = data[i] * a;
	return temp;
}
kl::mat4 kl::mat4::operator*(float a) const
{
	return this->mul(a);
}
void kl::mat4::operator*=(float a)
{
	for (int i = 0; i < 16; i++)
		data[i] *= a;
}
kl::mat4 kl::mat4::mul(const kl::mat4& obj) const
{
	kl::mat4 temp;
	temp[0] = data[0] * obj.data[0] + data[1] * obj.data[4] + data[2] * obj.data[8] + data[3] * obj.data[12];
	temp[1] = data[0] * obj.data[1] + data[1] * obj.data[5] + data[2] * obj.data[9] + data[3] * obj.data[13];
	temp[2] = data[0] * obj.data[2] + data[1] * obj.data[6] + data[2] * obj.data[10] + data[3] * obj.data[14];
	temp[3] = data[0] * obj.data[3] + data[1] * obj.data[7] + data[2] * obj.data[11] + data[3] * obj.data[15];
	temp[4] = data[4] * obj.data[0] + data[5] * obj.data[4] + data[6] * obj.data[8] + data[7] * obj.data[12];
	temp[5] = data[4] * obj.data[1] + data[5] * obj.data[5] + data[6] * obj.data[9] + data[7] * obj.data[13];
	temp[6] = data[4] * obj.data[2] + data[5] * obj.data[6] + data[6] * obj.data[10] + data[7] * obj.data[14];
	temp[7] = data[4] * obj.data[3] + data[5] * obj.data[7] + data[6] * obj.data[11] + data[7] * obj.data[15];
	temp[8] = data[8] * obj.data[0] + data[9] * obj.data[4] + data[10] * obj.data[8] + data[11] * obj.data[12];
	temp[9] = data[8] * obj.data[1] + data[9] * obj.data[5] + data[10] * obj.data[9] + data[11] * obj.data[13];
	temp[10] = data[8] * obj.data[2] + data[9] * obj.data[6] + data[10] * obj.data[10] + data[11] * obj.data[14];
	temp[11] = data[8] * obj.data[3] + data[9] * obj.data[7] + data[10] * obj.data[11] + data[11] * obj.data[15];
	temp[12] = data[12] * obj.data[0] + data[13] * obj.data[4] + data[14] * obj.data[8] + data[15] * obj.data[12];
	temp[13] = data[12] * obj.data[1] + data[13] * obj.data[5] + data[14] * obj.data[9] + data[15] * obj.data[13];
	temp[14] = data[12] * obj.data[2] + data[13] * obj.data[6] + data[14] * obj.data[10] + data[15] * obj.data[14];
	temp[15] = data[12] * obj.data[3] + data[13] * obj.data[7] + data[14] * obj.data[11] + data[15] * obj.data[15];
	return temp;
}
kl::mat4 kl::mat4::operator*(const kl::mat4& obj) const
{
	return this->mul(obj);
}
void kl::mat4::operator*=(const kl::mat4& obj)
{
	*this = this->mul(obj);
}
kl::float4 kl::mat4::mul(const kl::float4& obj) const
{
	kl::float4 temp;
	temp.x = data[0] * obj.x + data[1] * obj.y + data[2] * obj.z + data[3] * obj.w;
	temp.y = data[4] * obj.x + data[5] * obj.y + data[6] * obj.z + data[7] * obj.w;
	temp.z = data[8] * obj.x + data[9] * obj.y + data[10] * obj.z + data[11] * obj.w;
	temp.w = data[12] * obj.x + data[13] * obj.y + data[14] * obj.z + data[15] * obj.w;
	return temp;
}
kl::float4 kl::mat4::operator*(const kl::float4& obj) const
{
	return this->mul(obj);
}

// Division
kl::mat4 kl::mat4::div(float a) const
{
	return this->mul(1 / a);
}
kl::mat4 kl::mat4::operator/(float a) const
{
	return this->div(a);
}
void kl::mat4::operator /= (float a)
{
	this->operator*=(1 / a);
}

// Comparison
bool kl::mat4::equals(const kl::mat4& obj) const
{
	for (int i = 0; i < 16; i++)
	{
		if (data[i] != obj.data[i])
			return false;
	}
	return true;
}
bool kl::mat4::operator==(const kl::mat4& obj) const
{
	return this->equals(obj);
}
bool kl::mat4::operator!=(const kl::mat4& obj) const
{
	return !this->equals(obj);
}

// Returns pointer to raw data
float* kl::mat4::pointer() const
{
	return (float*)data;
}

// Loads the identity matrix
void kl::mat4::setIdentity()
{
	data[0] = 1; data[1] = 0; data[2] = 0; data[3] = 0;
	data[4] = 0; data[5] = 1; data[6] = 0; data[7] = 0;
	data[8] = 0; data[9] = 0; data[10] = 1; data[11] = 0;
	data[12] = 0; data[13] = 0; data[14] = 0; data[15] = 1;
}

// Returns inverse matrix if possible
kl::mat4 kl::mat4::inverse() const
{
	kl::mat4 inv;
	inv[0] = data[5] * data[10] * data[15] -
		data[5] * data[11] * data[14] -
		data[9] * data[6] * data[15] +
		data[9] * data[7] * data[14] +
		data[13] * data[6] * data[11] -
		data[13] * data[7] * data[10];
	inv[4] = -data[4] * data[10] * data[15] +
		data[4] * data[11] * data[14] +
		data[8] * data[6] * data[15] -
		data[8] * data[7] * data[14] -
		data[12] * data[6] * data[11] +
		data[12] * data[7] * data[10];
	inv[8] = data[4] * data[9] * data[15] -
		data[4] * data[11] * data[13] -
		data[8] * data[5] * data[15] +
		data[8] * data[7] * data[13] +
		data[12] * data[5] * data[11] -
		data[12] * data[7] * data[9];
	inv[12] = -data[4] * data[9] * data[14] +
		data[4] * data[10] * data[13] +
		data[8] * data[5] * data[14] -
		data[8] * data[6] * data[13] -
		data[12] * data[5] * data[10] +
		data[12] * data[6] * data[9];
	inv[1] = -data[1] * data[10] * data[15] +
		data[1] * data[11] * data[14] +
		data[9] * data[2] * data[15] -
		data[9] * data[3] * data[14] -
		data[13] * data[2] * data[11] +
		data[13] * data[3] * data[10];
	inv[5] = data[0] * data[10] * data[15] -
		data[0] * data[11] * data[14] -
		data[8] * data[2] * data[15] +
		data[8] * data[3] * data[14] +
		data[12] * data[2] * data[11] -
		data[12] * data[3] * data[10];
	inv[9] = -data[0] * data[9] * data[15] +
		data[0] * data[11] * data[13] +
		data[8] * data[1] * data[15] -
		data[8] * data[3] * data[13] -
		data[12] * data[1] * data[11] +
		data[12] * data[3] * data[9];
	inv[13] = data[0] * data[9] * data[14] -
		data[0] * data[10] * data[13] -
		data[8] * data[1] * data[14] +
		data[8] * data[2] * data[13] +
		data[12] * data[1] * data[10] -
		data[12] * data[2] * data[9];
	inv[2] = data[1] * data[6] * data[15] -
		data[1] * data[7] * data[14] -
		data[5] * data[2] * data[15] +
		data[5] * data[3] * data[14] +
		data[13] * data[2] * data[7] -
		data[13] * data[3] * data[6];
	inv[6] = -data[0] * data[6] * data[15] +
		data[0] * data[7] * data[14] +
		data[4] * data[2] * data[15] -
		data[4] * data[3] * data[14] -
		data[12] * data[2] * data[7] +
		data[12] * data[3] * data[6];
	inv[10] = data[0] * data[5] * data[15] -
		data[0] * data[7] * data[13] -
		data[4] * data[1] * data[15] +
		data[4] * data[3] * data[13] +
		data[12] * data[1] * data[7] -
		data[12] * data[3] * data[5];
	inv[14] = -data[0] * data[5] * data[14] +
		data[0] * data[6] * data[13] +
		data[4] * data[1] * data[14] -
		data[4] * data[2] * data[13] -
		data[12] * data[1] * data[6] +
		data[12] * data[2] * data[5];
	inv[3] = -data[1] * data[6] * data[11] +
		data[1] * data[7] * data[10] +
		data[5] * data[2] * data[11] -
		data[5] * data[3] * data[10] -
		data[9] * data[2] * data[7] +
		data[9] * data[3] * data[6];
	inv[7] = data[0] * data[6] * data[11] -
		data[0] * data[7] * data[10] -
		data[4] * data[2] * data[11] +
		data[4] * data[3] * data[10] +
		data[8] * data[2] * data[7] -
		data[8] * data[3] * data[6];
	inv[11] = -data[0] * data[5] * data[11] +
		data[0] * data[7] * data[9] +
		data[4] * data[1] * data[11] -
		data[4] * data[3] * data[9] -
		data[8] * data[1] * data[7] +
		data[8] * data[3] * data[5];
	inv[15] = data[0] * data[5] * data[10] -
		data[0] * data[6] * data[9] -
		data[4] * data[1] * data[10] +
		data[4] * data[2] * data[9] +
		data[8] * data[1] * data[6] -
		data[8] * data[2] * data[5];

	// Det calc
	const float det = data[0] * inv[0] + data[1] * inv[4] + data[2] * inv[8] + data[3] * inv[12];

	// Det check
	if (det == 0.0f)
		return kl::mat4();

	// Return
	return inv.div(det);
}

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, kl::mat4& obj)
{
	os <<
		kl::convert::toString(obj[0]) << " " << kl::convert::toString(obj[1]) << " " << kl::convert::toString(obj[2]) << " " << kl::convert::toString(obj[3]) << "\n" <<
		kl::convert::toString(obj[4]) << " " << kl::convert::toString(obj[5]) << " " << kl::convert::toString(obj[6]) << " " << kl::convert::toString(obj[7]) << "\n" <<
		kl::convert::toString(obj[8]) << " " << kl::convert::toString(obj[9]) << " " << kl::convert::toString(obj[10]) << " " << kl::convert::toString(obj[11]) << "\n" <<
		kl::convert::toString(obj[12]) << " " << kl::convert::toString(obj[13]) << " " << kl::convert::toString(obj[14]) << " " << kl::convert::toString(obj[15]);
	return os;
}
std::ostream& kl::operator<<(std::ostream& os, kl::mat4&& obj)
{
	os <<
		kl::convert::toString(obj[0]) << " " << kl::convert::toString(obj[1]) << " " << kl::convert::toString(obj[2]) << " " << kl::convert::toString(obj[3]) << "\n" <<
		kl::convert::toString(obj[4]) << " " << kl::convert::toString(obj[5]) << " " << kl::convert::toString(obj[6]) << " " << kl::convert::toString(obj[7]) << "\n" <<
		kl::convert::toString(obj[8]) << " " << kl::convert::toString(obj[9]) << " " << kl::convert::toString(obj[10]) << " " << kl::convert::toString(obj[11]) << "\n" <<
		kl::convert::toString(obj[12]) << " " << kl::convert::toString(obj[13]) << " " << kl::convert::toString(obj[14]) << " " << kl::convert::toString(obj[15]);
	return os;
}

// Returns a translation matrix
kl::mat4 kl::mat4::translate(const kl::float3& translation)
{
	kl::mat4 temp;
	temp[3] = translation.x;
	temp[7] = translation.y;
	temp[11] = translation.z;
	return temp;
}

// Returns a rotation matrix
kl::mat4 kl::mat4::rotate(const kl::float3& rotation)
{
	// Computing x rotation matrix
	const float xRad = kl::convert::toRadians(rotation.x);
	const float xSin = sin(xRad);
	const float xCos = cos(xRad);
	kl::mat4 xRot;
	xRot[5] = xCos;
	xRot[6] = -xSin;
	xRot[9] = xSin;
	xRot[10] = xCos;

	// Computing y rotation matrix
	const float yRad = kl::convert::toRadians(rotation.y);
	const float ySin = sin(yRad);
	const float yCos = cos(yRad);
	kl::mat4 yRot;
	yRot[0] = yCos;
	yRot[2] = ySin;
	yRot[8] = -ySin;
	yRot[10] = yCos;

	// Computing z rotation matrix
	const float zRad = kl::convert::toRadians(rotation.z);
	const float zSin = sin(zRad);
	const float zCos = cos(zRad);
	kl::mat4 zRot;
	zRot[0] = zCos;
	zRot[1] = -zSin;
	zRot[4] = zSin;
	zRot[5] = zCos;

	// Multiplying and returning
	return zRot * yRot * xRot;
}

// Returns a scaling matrix
kl::mat4 kl::mat4::scale(const kl::float3& size)
{
	kl::mat4 temp;
	temp[0] = size.x;
	temp[5] = size.y;
	temp[10] = size.z;
	return temp;
}

// Returns the perspective projection matrix
kl::mat4 kl::mat4::persp(float fov, float ar, float zNear, float zFar)
{
	const float tanHalf = 1 / tan(kl::convert::toRadians(fov) * 0.5f);
	kl::mat4 temp;
	temp[0] = tanHalf / ar;
	temp[5] = tanHalf;
	temp[10] = (-zFar - zNear) / (zNear - zFar);
	temp[11] = (2 * zNear * zFar) / (zNear - zFar);
	temp[14] = 1;
	temp[15] = 0;
	return temp;
}

// Returns the orthographics projection matrix
kl::mat4 kl::mat4::ortho(float left, float right, float bottom, float top, float nearZ, float farZ)
{
	kl::mat4 temp;
	temp[0] = 2 / (right - left);
	temp[5] = 2 / (top - bottom);
	temp[10] = -2 / (farZ - nearZ);
	temp[3] = -(right + left) / (right - left);
	temp[7] = -(top + bottom) / (top - bottom);
	temp[11] = -(farZ + nearZ) / (farZ - nearZ);
	return temp;
}

// Returns the "look at" matrix
kl::mat4 kl::mat4::lookAt(const kl::float3& position, const kl::float3& target, const kl::float3& up)
{
	// Vector calculations
	const kl::float3 f = (target - position).normalize();
	const kl::float3 s = up.cross(f).normalize();
	const kl::float3 u = f.cross(s);

	// Matrix building
	kl::mat4 temp;
	temp[0] = s.x;
	temp[1] = s.y;
	temp[2] = s.z;
	temp[3] = -s.dot(position);
	temp[4] = u.x;
	temp[5] = u.y;
	temp[6] = u.z;
	temp[7] = -u.dot(position);
	temp[8] = f.x;
	temp[9] = f.y;
	temp[10] = f.z;
	temp[11] = -f.dot(position);
	return temp;
}
