#include "GPU/GPU.h"

#include "Utility/Console.h"


kl::dx::Buffer kl::GPU::newBuffer(dx::BufferDesc* descriptor, dx::SubresDesc* subresourceData) {
	dx::Buffer buffer = nullptr;

	m_Device->CreateBuffer(descriptor, subresourceData, &buffer);
	if (Warning(!buffer, "Failed to create gpu buffer")) {
		return nullptr;
	}

	m_Children.insert(buffer);

	return buffer;
}
