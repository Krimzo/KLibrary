#include "gpu/gpu.h"

#include "utility/console.h"


kl::dx::buffer kl::gpu::newBuffer(kl::dx::desc::buffer* desc, kl::dx::desc::subres* subData) {
	kl::dx::buffer buff = nullptr;
	m_Device->CreateBuffer(desc, subData, &buff);
	kl::console::error(!buff, "Failed to create gpu buffer");

	m_Children.insert(buff);
	return buff;
}
