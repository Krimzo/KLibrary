#include "KrimzLib/gl/uniform.h"


// Constructors
kl::uniform::uniform() {
	this->programID = NULL;
	this->uniformID = NULL;
}
kl::uniform::uniform(kl::id programID, kl::id uniformID) {
	this->programID = programID;
	this->uniformID = uniformID;
}

// Setts the data of an uniform
void kl::uniform::setData(int data) const {
	glUseProgram(programID);
	glUniform1i(uniformID, data);
}
void kl::uniform::setData(float data) const {
	glUseProgram(programID);
	glUniform1f(uniformID, data);
}
void kl::uniform::setData(const kl::vec2& data) const {
	glUseProgram(programID);
	glUniform2f(uniformID, data.x, data.y);
}
void kl::uniform::setData(const kl::vec3& data) const {
	glUseProgram(programID);
	glUniform3f(uniformID, data.x, data.y, data.z);
}
void kl::uniform::setData(const kl::vec4& data) const {
	glUseProgram(programID);
	glUniform4f(uniformID, data.x, data.y, data.z, data.w);
}
void kl::uniform::setData(const kl::mat3& data) const {
	glUseProgram(programID);
	glUniformMatrix3fv(uniformID, 1, true, data.pointer());
}
void kl::uniform::setData(const kl::mat4& data) const {
	glUseProgram(programID);
	glUniformMatrix4fv(uniformID, 1, true, data.pointer());
}
