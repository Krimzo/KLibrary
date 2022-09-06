#include "GPU/GPU.h"

#include "Utility/Console.h"


kl::dx::Buffer kl::GPU::newBuffer(dx::BufferDesc* descriptor, dx::SubresDesc* subresourceData) {
	dx::Buffer buffer = nullptr;

	long result = m_Device->CreateBuffer(descriptor, subresourceData, &buffer);
	if (Warning(!buffer, Format("Failed to create gpu buffer. Result: 0x", std::hex, result))) {
		return nullptr;
	}

	m_Children.insert(buffer);

	return buffer;
}

kl::dx::Buffer kl::GPU::newStructuredBuffer(const void* data, uint elementCount, uint elementSize, bool hasUnorderedAccess, bool cpuRead) {
	dx::BufferDesc descriptor = {};
	descriptor.Usage = D3D11_USAGE_DEFAULT;
	descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE | (hasUnorderedAccess ? D3D11_BIND_UNORDERED_ACCESS : NULL);
	descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	descriptor.CPUAccessFlags = cpuRead ? D3D11_CPU_ACCESS_READ : NULL;
	descriptor.StructureByteStride = elementSize;
	descriptor.ByteWidth = elementCount * elementSize;

	dx::SubresDesc subresource = {};
	subresource.pSysMem = data;

	return newBuffer(&descriptor, &subresource);
}

kl::dx::Buffer kl::GPU::newStagingBuffer(dx::Buffer buffer, uint byteSize) {
	dx::BufferDesc descriptor = {};
	buffer->GetDesc(&descriptor);

	dx::BufferDesc stagingDescriptor = {};
	stagingDescriptor.Usage = D3D11_USAGE_STAGING;
	stagingDescriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	stagingDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagingDescriptor.StructureByteStride = descriptor.StructureByteStride;
	stagingDescriptor.ByteWidth = byteSize ? byteSize : descriptor.ByteWidth;

	return newBuffer(&stagingDescriptor);
}
