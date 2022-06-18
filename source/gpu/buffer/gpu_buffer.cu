#include "gpu/gpu.cuh"

#include "utility/console.cuh"


kl::dx::buffer kl::gpu::newBuffer(kl::dx::desc::buffer* desc, kl::dx::desc::subres* subData) {
	kl::dx::buffer m_Buffer = nullptr;
	m_Device->CreateBuffer(desc, subData, &m_Buffer);
	kl::console::error(!m_Buffer, "Failed to create gpu buffer");

	m_Children.insert(m_Buffer);
	return m_Buffer;
}
