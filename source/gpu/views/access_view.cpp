#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11UnorderedAccessView* kl::gpu::newAccessView(ID3D11Texture2D* tex, D3D11_UNORDERED_ACCESS_VIEW_DESC* desc) {
	// Creating the render target view
	ID3D11UnorderedAccessView* accessView = nullptr;
	device->CreateUnorderedAccessView(tex, desc, &accessView);
	if (!accessView) {
		kl::console::show();
		std::cout << "DirectX: Could not create an unordered access view!";
		std::cin.get();
		exit(69);
	}

	// Saving child
	children.insert(accessView);

	// Return
	return accessView;
}

// Binds the view
void kl::gpu::bindComputeAccessView(ID3D11UnorderedAccessView* view, uint32_t slot, uint32_t* initalCounts) {
	devcon->CSSetUnorderedAccessViews(slot, 1, &view, initalCounts);
}
