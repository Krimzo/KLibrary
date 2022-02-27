#include "KrimzLib/KrimzLib.hpp"


struct PS_CB {
	kl::vec4 frameSize;
	kl::vec4 camPos;
	kl::vec4 objCol;
	kl::vec4 vertCount;
	kl::mat4 ivp;
	kl::mat4 w;
};

int main() {
	// Win
	kl::window win;

	// Gpu
	kl::gpu* gpu = nullptr;
	kl::shaders* raytracing_sh = nullptr;

	// Time
	kl::timer timer;
	
	// View
	kl::camera camera;

	// Mesh
	kl::mesh* screen_mes = nullptr;

	// Obj data
	kl::vec3 cubeRotation;
	size_t cubeVertCount = 0;
	
	// Start
	win.start = [&]() {
		// Gpu creation
		gpu = new kl::gpu(win.getWND());
		
		// Gpu raster and depth testing setup
		gpu->newRaster(false, false)->bind();
		gpu->setDSState(kl::dbuffer::Disabled);

		// Shader compilation
		raytracing_sh = gpu->newShaders("res/shaders/raytracing.hlsl", 0, sizeof(PS_CB));
		raytracing_sh->bind();

		// Screen mesh creation
		screen_mes = gpu->newMesh({
			kl::vertex(kl::vec3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::vec3(-1.0f,  1.0f, 0.5f)), kl::vertex(kl::vec3(1.0f, 1.0f, 0.5f)),
			kl::vertex(kl::vec3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::vec3( 1.0f, -1.0f, 0.5f)), kl::vertex(kl::vec3(1.0f, 1.0f, 0.5f))
		});
		
		// Sampler creation
		gpu->newSampler(false, false)->bind(0);

		/* TEXTURE TEST */
		// Getting the vertex data
		std::vector<kl::vertex> katanaFullData = kl::file::parseObj("res/objects/cube.obj");

		// Extracting the world positions and flipping
		std::vector<kl::vec3> katanaVertices(katanaFullData.size());
		for (int i = 0; i < katanaVertices.size(); i++) {
			katanaVertices[i] = katanaFullData[i].world;
		}

		// Saving the vertex count
		cubeVertCount = katanaVertices.size();

		// Texture descriptor creation
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = 3;
		texDesc.Height = UINT(katanaVertices.size() / 3);
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		// Texture data descriptor creation
		D3D11_SUBRESOURCE_DATA texData = {};
		texData.pSysMem = &katanaVertices[0];
		texData.SysMemPitch = UINT(3 * sizeof(kl::vec3));

		// Texture creation
		ID3D11Texture2D* tex = nullptr;
		gpu->getDev()->CreateTexture2D(&texDesc, &texData, &tex);
		if (!tex) {
			std::cout << "DirectX: Could not create a 2D texture!";
			std::cin.get();
			exit(69);
		}

		// Texture view creation
		ID3D11ShaderResourceView* view = nullptr;
		gpu->getDev()->CreateShaderResourceView(tex, nullptr, &view);
		if (!view) {
			std::cout << "DirectX: Could not create a 2D texture view!";
			std::cin.get();
			exit(69);
		}

		// Cleanup
		tex->Release();

		// Texture bind
		gpu->getCon()->PSSetShaderResources(0, 1, &view);

		/* ------------ */
	};

	// Update
	win.update = [&]() {
		// Time
		const float deltaT = timer.interval();

		// Physics
		cubeRotation.y += 18.0f * deltaT;
		camera.position.z = -3.0f;

		// Setting the pixel shader data
		PS_CB psData = {};
		psData.frameSize = kl::vec4(win.getSize(), 0.0f, 0.0f);
		psData.camPos = camera.position;
		psData.objCol = kl::colors::wheat;
		psData.vertCount = kl::vec4(float(cubeVertCount));
		psData.ivp = camera.matrix().inverse();
		psData.w = kl::mat4::rotate(cubeRotation);
		raytracing_sh->setPixlData(&psData);

		// Raytracing
		screen_mes->draw();

		// Buffer swap
		gpu->swap(true);

		// Fps display
		win.setTitle(std::to_string(int(1 / deltaT)));
	};

	timer.interval();
	win.startNew(kl::ivec2(1600, 900), "xD", false, true);
}
